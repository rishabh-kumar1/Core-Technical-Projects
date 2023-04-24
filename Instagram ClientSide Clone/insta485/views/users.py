"""Insta485 USERs (main) view.

URLs include:
/
"""
import os
import pathlib
import uuid
from flask import abort
import flask
import insta485


@insta485.app.route('/users/<user_url_slug>/', methods=['GET'])
def get_users_slug(user_url_slug):
    """Get users slug."""
    if 'username' not in flask.session:
        return flask.redirect(flask.url_for('show_login_get'))
    connection = insta485.model.get_db()

    cur = connection.execute(
        "SELECT username "
        "FROM users "
        "WHERE username = ?",
        (user_url_slug,)
    )
    result = cur.fetchall()

    if len(result) == 0:
        abort(404)

    logname = flask.session['username']
    context = {"logname": logname}
    context["username"] = user_url_slug

    curry = connection.execute(
        "SELECT username2 "
        "FROM following "
        "WHERE username1 = ?",
        (logname, )
    )
    result_log = curry.fetchall()
    context["logname_follows_username"] = False
    for res in result_log:
        if res["username2"] == user_url_slug:
            context["logname_follows_username"] = True
    cur = connection.execute(
        "SELECT fullname "
        "FROM users "
        "WHERE username = ?",
        (user_url_slug, )
    )
    result = cur.fetchall()
    context["fullname"] = result[0]["fullname"]

    cur = connection.execute(
        "SELECT username2 "
        "FROM following "
        "WHERE username1 = ?",
        (user_url_slug, )
    )
    result = cur.fetchall()
    context["following"] = len(result)

    cur = connection.execute(
        "SELECT username1 "
        "FROM following "
        "WHERE username2 = ?",
        (user_url_slug, )
    )
    result = cur.fetchall()
    context["followers"] = len(result)

    cur = connection.execute(
        "SELECT postid, filename "
        "FROM posts "
        "WHERE owner = ?",
        (user_url_slug, )
    )
    result = cur.fetchall()
    context["total_posts"] = len(result)
    context["posts"] = []
    for res in result:
        res["postid"] = str(res["postid"])
        context["posts"].append(res)
    context["posts"] = sorted(context["posts"], key=lambda d: d['postid'])
    return flask.render_template("user.html", **context)


@insta485.app.route('/users/<user_url_slug>/followers/', methods=['GET'])
def get_users_followers(user_url_slug):
    """Get users followers."""
    if 'username' not in flask.session:
        return flask.redirect(flask.url_for('show_login_get'))
    connection = insta485.model.get_db()
    cur = connection.execute(
        "SELECT username "
        "FROM users "
        "WHERE username = ?",
        (user_url_slug,)
    )
    result = cur.fetchall()
    if len(result) == 0:
        abort(404)

    logname = flask.session['username']
    context = {"logname": logname}
    context["curr_user"] = user_url_slug
    context["followers"] = []

    cur = connection.execute(
        "SELECT username1 "
        "FROM following "
        "WHERE username2 = ?",
        (user_url_slug, )
    )
    result = cur.fetchall()
    for res in result:
        res["username"] = res["username1"]
        cur = connection.execute(
            "SELECT filename "
            "FROM users "
            "WHERE username = ?",
            (res["username"], )
        )
        inner_result = cur.fetchall()
        res["user_img_url"] = inner_result[0]["filename"]

        cur = connection.execute(
            "SELECT username1 "
            "FROM following "
            "WHERE username2 = ?",
            (res["username"], )
        )
        inner_result = cur.fetchall()
        res["logname_follows_username"] = False
        for i in inner_result:
            if i["username1"] == logname:
                res["logname_follows_username"] = True
        context["followers"].append(res)
    return flask.render_template("followers.html", **context)


@insta485.app.route('/users/<user_url_slug>/following/', methods=['GET'])
def get_users_following(user_url_slug):
    """Get users following."""
    if 'username' not in flask.session:
        return flask.redirect(flask.url_for('show_login_get'))

    connection = insta485.model.get_db()

    cur = connection.execute(
        "SELECT username "
        "FROM users "
        "WHERE username = ?",
        (user_url_slug,)
    )
    result = cur.fetchall()
    if len(result) == 0:
        abort(404)

    logname = flask.session['username']
    context = {"logname": logname}
    context["curr_user"] = user_url_slug
    context["following"] = []

    cur = connection.execute(
        "SELECT username2 "
        "FROM following "
        "WHERE username1 = ?",
        (user_url_slug, )
    )
    result = cur.fetchall()
    for res in result:
        res["username"] = res["username2"]
        cur = connection.execute(
            "SELECT filename "
            "FROM users "
            "WHERE username = ?",
            (res["username"], )
        )
        inner_result = cur.fetchall()
        res["user_img_url"] = inner_result[0]["filename"]

        cur = connection.execute(
            "SELECT username1 "
            "FROM following "
            "WHERE username2 = ?",
            (res["username"], )
        )
        inner_result = cur.fetchall()
        res["logname_follows_username"] = False
        for i in inner_result:
            if i["username1"] == logname:
                res["logname_follows_username"] = True
        context["following"].append(res)
    return flask.render_template("following.html", **context)


@insta485.app.route('/likes/', methods=['POST'])
def show_likes():
    """Show likes."""
    if 'username' not in flask.session:
        return flask.redirect(flask.url_for('show_login_get'))

    operation = flask.request.form['operation']
    connection = insta485.model.get_db()
    username = flask.session['username']
    postid = flask.request.form['postid']
    if operation == "like":
        cur = connection.execute(
            "SELECT likeid "
            "FROM likes "
            "WHERE owner = ? AND postid = ? ",
            (username, postid)
        )
        liked = cur.fetchall()
        if len(liked) > 0:
            abort(409)

        cursor = connection.cursor()
        cursor.execute(
            "INSERT INTO likes (owner, postid) VALUES (?, ?)",
            (username, postid)
        )
        connection.commit()

    elif operation == "unlike":
        cur = connection.execute(
            "SELECT likeid "
            "FROM likes "
            "WHERE owner = ? AND postid = ? ",
            (username, postid)
        )
        liked = cur.fetchall()
        if len(liked) == 0:
            abort(409)

        cursor = connection.cursor()
        cursor.execute(
            "DELETE FROM likes WHERE owner = ? AND postid = ?",
            (username, postid)
        )
        connection.commit()

    if (direction := flask.request.args.get("target")):
        return flask.redirect(direction)
    return flask.redirect(flask.url_for('show_index'))


@insta485.app.route('/comments/', methods=['POST'])
def show_comments():
    """Show comments."""
    if 'username' not in flask.session:
        return flask.redirect(flask.url_for('show_login_get'))

    operation = flask.request.form['operation']
    connection = insta485.model.get_db()
    username = flask.session['username']

    if operation == "create":
        postid = flask.request.form['postid']
        text = flask.request.form['text']
        if len(text) == 0:
            abort(400)

        cursor = connection.cursor()
        cursor.execute(
            "INSERT INTO comments (owner, postid, text) VALUES (?, ?, ?)",
            (username, postid, text)
        )
        connection.commit()
    elif operation == "delete":
        commentid = flask.request.form['commentid']
        cur = connection.execute(
            "SELECT owner "
            "FROM comments "
            "WHERE commentid = ? ",
            (commentid)
        )
        comid = cur.fetchone()['owner']
        if comid != username:
            abort(400)

        cursor = connection.cursor()
        cursor.execute(
            "DELETE FROM comments WHERE owner = ? AND commentid = ?",
            (username, commentid)
        )
        connection.commit()

    if (direction := flask.request.args.get("target")):
        return flask.redirect(direction)
    return flask.redirect(flask.url_for('show_index'))


@insta485.app.route('/posts/', methods=['POST'])
def show_posts():
    """Show posts."""
    if 'username' not in flask.session:
        return flask.redirect(flask.url_for('show_login_get'))

    operation = flask.request.form['operation']
    connection = insta485.model.get_db()
    username = flask.session['username']
    if operation == "create":
        fileobj = flask.request.files['file']
        if not fileobj:
            abort(400)

        filename = fileobj.filename
        stem = uuid.uuid4().hex
        suffix = pathlib.Path(filename).suffix
        uuid_basename = f"{stem}{suffix}"
        # Save to disk
        path = insta485.app.config["UPLOAD_FOLDER"]/uuid_basename
        fileobj.save(path)

        cursor = connection.cursor()
        cursor.execute(
            "INSERT INTO posts (filename, owner) VALUES (?, ?)",
            (uuid_basename, username)
        )
        connection.commit()
    elif operation == "delete":
        if 'username' not in flask.session:
            return flask.redirect(flask.url_for('show_login_get'))
        postid = flask.request.form['postid']

        cur = connection.execute(
            "SELECT filename "
            "FROM posts "
            "WHERE owner = ? AND postid = ? ",
            (username, postid)
        )
        filename = cur.fetchone()['filename']

        cur = connection.execute(
            "SELECT owner "
            "FROM posts "
            "WHERE filename = ? AND postid = ? ",
            (filename, postid)
        )
        postcheck = cur.fetchone()['owner']

        if postcheck != username:
            abort(403)

        path = insta485.app.config["UPLOAD_FOLDER"]/filename
        os.remove(path)

        cursor = connection.cursor()
        cursor.execute("DELETE FROM posts WHERE postid = ?", (postid, ))
        connection.commit()

        cursor = connection.cursor()
        cursor.execute("DELETE FROM comments WHERE postid = ?", (postid, ))
        connection.commit()

        cursor = connection.cursor()
        cursor.execute("DELETE FROM likes WHERE postid = ?", (postid, ))
        connection.commit()

    if (direction := flask.request.args.get("target")):
        return flask.redirect(direction)
    return flask.redirect(
        flask.url_for('get_users_slug', user_url_slug=username)
    )


@insta485.app.route('/following/', methods=['POST'])
def show_following():
    """Show following."""
    logname = flask.session['username']
    operation = flask.request.form["operation"]
    username = flask.request.form["username"]
    connection = insta485.model.get_db()
    if operation == "follow":
        cur = connection.execute(
            "SELECT username1 "
            "FROM following "
            "WHERE username2 = ?",
            (username, )
        )
        result = cur.fetchall()
        for res in result:
            if res["username1"] == logname:
                abort(409)
        cursor = connection.cursor()
        cursor.execute(
            "INSERT INTO following (username1, username2) VALUES (?, ?)",
            (logname, username)
        )
        connection.commit()

    elif operation == "unfollow":
        cur = connection.execute(
            "SELECT username1 "
            "FROM following "
            "WHERE username2 = ?",
            (username, )
        )
        result = cur.fetchall()

        bool_unfollow = True
        for res in result:
            if res["username1"] == logname:
                bool_unfollow = False
        if bool_unfollow is True:
            abort(409)

        if len(result) == 0:
            abort(409)

        cursor = connection.cursor()
        cursor.execute(
            "DELETE FROM following "
            "WHERE username1 = ? AND username2 = ?",
            (logname, username)
        )
        connection.commit()
    if (direction := flask.request.args.get("target")):
        return flask.redirect(direction)
    return flask.redirect(flask.url_for('show_index'))
