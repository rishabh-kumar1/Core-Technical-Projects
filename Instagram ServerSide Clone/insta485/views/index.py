"""Insta485 index (main) view.

URLs include:
/
"""
import flask
import arrow
import insta485


@insta485.app.route('/')
def show_index():
    """Display / route."""
    connection = insta485.model.get_db()

    if 'username' not in flask.session:
        return flask.redirect(flask.url_for('show_login_get'))
    logname = flask.session['username']
    # Connect to database
    connection = insta485.model.get_db()
    # Query database
    cur = connection.execute(
        "SELECT username2 "
        "FROM following "
        "WHERE username1 = ?",
        (logname, )
    )
    result = cur.fetchall()
    total_users = []
    for u_r in result:
        total_users.append(u_r['username2'])
    total_users.append(logname)

    context = {"logname": logname}
    context["posts"] = []

    for u_r in total_users:
        cur = connection.execute(
            "SELECT filename, postid, owner, created "
            "FROM posts "
            "WHERE owner = ?",
            (u_r, )
        )
        result = cur.fetchall()
        for res in result:
            cur = connection.execute(
                "SELECT filename "
                "FROM users "
                "WHERE username = ?",
                (u_r, )
            )
            name_total = cur.fetchall()
            if len(name_total) > 0:
                res["owner_img_url"] = name_total[0]["filename"]
            cur = connection.execute(
                "SELECT likeid "
                "FROM likes "
                "WHERE postid = ?",
                (res["postid"], )
            )
            likes_total = cur.fetchall()
            if len(likes_total) > 0:
                res["likes"] = len(likes_total)
            else:
                res["likes"] = 0

            cur = connection.execute(
                "SELECT commentid, owner, text "
                "FROM comments "
                "WHERE postid = ?",
                (res["postid"], )
            )

            res["postid"] = str(res["postid"])
            comments_total = cur.fetchall()
            res["comments"] = []
            human_time = arrow.get(res["created"])
            res["timestamp"] = human_time.humanize()

            res["hasliked"] = False
            cur = connection.execute(
                "SELECT likeid "
                "FROM likes "
                "WHERE owner = ? AND postid = ? ",
                (context["logname"], res["postid"],)
            )
            result = cur.fetchall()
            if len(result) > 0:
                res["hasliked"] = True

            for comm in comments_total:
                res["comments"].append(comm)

            res["comments"] = sorted(
                res["comments"], key=lambda d: d['commentid']
            )
            context["posts"].append(res)
    context["posts"] = sorted(
        context["posts"], key=lambda d: d['postid'], reverse=True
    )
    return flask.render_template("index.html", **context)


@insta485.app.route('/posts/<postid_url_slug>/', methods=['GET'])
def get_postid_url_slug(postid_url_slug):
    """GET POSTID URSL SLUG."""
    if 'username' not in flask.session:
        return flask.redirect(flask.url_for('show_login_get'))

    # Connect to database
    connection = insta485.model.get_db()
    # Query database
    logname = flask.session['username']

    context = {"logname": logname}

    cur = connection.execute(
        "SELECT filename, postid, owner, created "
        "FROM posts "
        "WHERE postid = ?",
        (postid_url_slug, )
    )
    result = cur.fetchall()

    context["owner"] = result[0]["owner"]
    context["postid"] = result[0]["postid"]
    context["img_url"] = result[0]["filename"]

    human_time = arrow.get(result[0]["created"])
    context["timestamp"] = human_time.humanize()

    cur = connection.execute(
        "SELECT filename "
        "FROM users "
        "WHERE username = ?",
        (context["owner"], )
    )
    result = cur.fetchall()

    if len(result) > 0:
        context["owner_img_url"] = result[0]["filename"]
    cur = connection.execute(
        "SELECT likeid "
        "FROM likes "
        "WHERE postid = ?",
        (context["postid"], )
    )
    result = cur.fetchall()
    if len(result) > 0:
        context["likes"] = len(result)
    else:
        context["likes"] = 0

    cur = connection.execute(
        "SELECT commentid, owner, text "
        "FROM comments "
        "WHERE postid = ?",
        (context["postid"], )
    )

    context["postid"] = str(context["postid"])

    result = cur.fetchall()
    context["comments"] = []

    for res in result:
        context["comments"].append(res)
    context["comments"] = sorted(
        context["comments"], key=lambda d: d['commentid']
    )

    context["hasliked"] = False
    cur = connection.execute(
        "SELECT likeid "
        "FROM likes "
        "WHERE owner = ? AND postid = ? ",
        (context["logname"], context["postid"],)
    )
    result = cur.fetchall()
    if len(result) > 0:
        context["hasliked"] = True
    return flask.render_template("post.html", **context)


@insta485.app.route('/explore/', methods=['GET'])
def explore():
    """EXPLORE."""
    if 'username' not in flask.session:
        return flask.redirect(flask.url_for('show_login_get'))
    connection = insta485.model.get_db()
    # Query database
    logname = flask.session['username']
    context = {"logname": logname}
    context["not_following"] = []
    cur = connection.execute(
        "SELECT username, filename "
        "FROM users "
        "WHERE username != ?",
        (logname, )
    )
    result = cur.fetchall()
    for res in result:
        cur = connection.execute(
            "SELECT username1 "
            "FROM following "
            "WHERE username2 = ? AND username1 = ?",
            (res["username"], logname)
        )
        inner_r = cur.fetchall()
        if len(inner_r) == 0:
            inner_r = {"username": res["username"]}
            inner_r["user_img_url"] = res["filename"]
            context["not_following"].append(inner_r)
        inner_r = cur.fetchall()
    return flask.render_template("explore.html", **context)
