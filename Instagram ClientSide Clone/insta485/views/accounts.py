"""Insta485 ACCOUNT (main) view.

URLs include:
/
"""
import pathlib
import uuid
import hashlib
import os
from flask import abort
import flask
import insta485


def hash_password(password):
    """HASH PASSWORD."""
    algorithm = 'sha512'
    salt = uuid.uuid4().hex
    hash_obj = hashlib.new(algorithm)
    password_salted = salt + password
    hash_obj.update(password_salted.encode('utf-8'))
    password_hash = hash_obj.hexdigest()
    password_db_string = "$".join([algorithm, salt, password_hash])
    return password_db_string


def decrypt_password(password, login):
    """DECRYPT/PASSOWRD."""
    idx1 = password.find('$')
    password = password[idx1+1:]
    idx2 = password.find('$')
    salt = password[:idx2]
    algorithm = 'sha512'
    hash_obj = hashlib.new(algorithm)
    password_salted = salt + login
    hash_obj.update(password_salted.encode('utf-8'))
    password_hash = hash_obj.hexdigest()
    password_db_string = "$".join([algorithm, salt, password_hash])
    return password_db_string


@insta485.app.route('/accounts/login/', methods=['GET'])
def show_login_get():
    """SHOW/login/get."""
    if 'username' in flask.session:
        return flask.redirect(flask.url_for('show_index'))
    return flask.render_template("login.html")


@insta485.app.route('/accounts/logout/', methods=['POST'])
def show_logout():
    """show/logout."""
    if 'username' not in flask.session:
        return flask.redirect(flask.url_for('show_login_get'))
    flask.session.clear()
    return flask.redirect(flask.url_for('show_login_get'))


@insta485.app.route('/accounts/create/', methods=['GET'])
def show_create():
    """show/create."""
    if 'username' in flask.session:
        return flask.redirect(flask.url_for('show_edit'))
    return flask.render_template("create.html")


@insta485.app.route('/accounts/delete/', methods=['GET'])
def show_delete():
    """show/delete."""
    if 'username' not in flask.session:
        return flask.redirect(flask.url_for('show_login_get'))
    logname = flask.session['username']
    context = {"logname": logname}
    return flask.render_template("delete.html", **context)


@insta485.app.route('/accounts/edit/', methods=['GET'])
def show_edit():
    """show/edit."""
    if 'username' not in flask.session:
        return flask.redirect(flask.url_for('show_login_get'))
    connection = insta485.model.get_db()
    logname = flask.session['username']
    cur = connection.execute(
        "SELECT fullname "
        "FROM users "
        "WHERE username = ?",
        (logname, )
    )
    result = cur.fetchone()
    context = {"fullname": result['fullname']}
    cur = connection.execute(
        "SELECT email "
        "FROM users "
        "WHERE username = ?",
        (logname, )
    )
    result = cur.fetchone()
    context['email'] = result['email']
    cur = connection.execute(
        "SELECT filename "
        "FROM users "
        "WHERE username = ?",
        (logname, )
    )
    result = cur.fetchone()
    context['logname'] = logname
    context['filename'] = result['filename']
    return flask.render_template("edit.html", **context)


@insta485.app.route('/uploads/<file>', methods=['GET'])
def show_uploads(file):
    """show/uploads."""
    if 'username' not in flask.session:
        abort(403)
    if not file:
        return ""

    return flask.send_from_directory(
        insta485.app.config['UPLOAD_FOLDER'], file
    )


@insta485.app.route('/accounts/password/', methods=['GET'])
def show_password():
    """show/password."""
    if 'username' not in flask.session:
        return flask.redirect(flask.url_for('show_login_get'))
    logname = flask.session['username']
    context = {"logname": logname}
    return flask.render_template("password.html", **context)


def do_login(login_username, login_password):
    """show_target login."""
    connection = insta485.model.get_db()
    if not login_username or not login_password:
        abort(400)
    cur = connection.execute(
        "SELECT username "
        "FROM users "
        "WHERE username = ?", (login_username,)
    )
    db_result = cur.fetchall()

    if len(db_result) == 0:
        abort(403)
    cur = connection.execute(
        "SELECT password "
        "FROM users "
        "WHERE username = ?",
        (login_username, )
    )
    db_result = cur.fetchall()
    db_result = db_result[0]['password']

    login_password = decrypt_password(db_result, login_password)
    if login_password != db_result:
        abort(403)
    flask.session['username'] = login_username


def do_delete():
    """show_target delete."""
    connection = insta485.model.get_db()
    if 'username' not in flask.session:
        abort(403)
    name = flask.session["username"]
    cur = connection.execute(
        "SELECT filename "
        "FROM users "
        "WHERE username = ? ", (flask.session['username'],)
    )
    oldfile = cur.fetchone()['filename']

    path = insta485.app.config["UPLOAD_FOLDER"]/oldfile
    os.remove(path)

    cur = connection.execute(
        "SELECT filename "
        "FROM posts "
        "WHERE owner = ? ", (flask.session['username'],)
    )
    oldposts = cur.fetchall()
    for i in oldposts:
        post = i['filename']
        path = insta485.app.config["UPLOAD_FOLDER"]/post
        os.remove(path)

    cursor = connection.cursor()
    cursor.execute("DELETE FROM users WHERE username = ?", (name, ))
    connection.commit()

    flask.session.clear()


def do_create(login_username, login_password,
              create_fullname, create_email, fileobj):
    """show_target create."""
    connection = insta485.model.get_db()
    if not login_username or not login_password or not create_fullname:
        abort(400)
    if not create_email or not fileobj:
        abort(400)
    cur = connection.execute(
        "SELECT username "
        "FROM users "
        "WHERE username = ?", (login_username,)
    )
    db_users = cur.fetchall()

    if len(db_users) > 0:
        abort(409)

    create_password = hash_password(login_password)
    fileobj = flask.request.files["file"]
    filename = fileobj.filename
    stema = uuid.uuid4().hex
    suffixa = pathlib.Path(filename).suffix
    uuid_basename = f"{stema}{suffixa}"
    patha = insta485.app.config["UPLOAD_FOLDER"]/uuid_basename
    fileobj.save(patha)

    cursor = connection.cursor()
    cursor.execute(
        "INSERT INTO users (username, fullname, email, filename, password)"
        "VALUES (?, ?, ?, ?, ?)",
        (
            login_username, create_fullname, create_email,
            filename, create_password
        )
    )

    connection.commit()
    flask.session['username'] = login_username


def do_edit(create_fullname, create_email, fileobj):
    """show_target edit."""
    connection = insta485.model.get_db()
    cur = connection.execute(
        "UPDATE users "
        "SET fullname = ?, "
        " email = ? "
        "WHERE username = ? ",
        (create_fullname, create_email, flask.session['username'],)
    )
    if fileobj:
        cur = connection.execute(
            "SELECT filename "
            "FROM users "
            "WHERE username = ? ", (flask.session['username'],)
        )
        oldfile = cur.fetchone()['filename']

        path = insta485.app.config["UPLOAD_FOLDER"]/oldfile
        print(path)
        os.remove(path)

        filename = fileobj.filename
        stema = uuid.uuid4().hex
        suffixa = pathlib.Path(filename).suffix
        uuid_basename = f"{stema}{suffixa}"
        # Save to disk
        patha = insta485.app.config["UPLOAD_FOLDER"]/uuid_basename
        fileobj.save(patha)
        print(uuid_basename)
        cur = connection.execute(
            "UPDATE users "
            "SET filename = ? "
            "WHERE username = ? ",
            (uuid_basename, flask.session['username'],)
        )


def do_password(oldpass, newpass1, newpass2):
    """show_target password."""
    connection = insta485.model.get_db()
    cur = connection.execute(
        "SELECT password "
        "FROM users "
        "WHERE username = ? ", (flask.session['username'],)
    )
    db_password = cur.fetchall()
    db_password = db_password[0]['password']
    print(db_password)
    oldpass = decrypt_password(db_password, oldpass)
    print(oldpass)
    if oldpass != db_password:
        abort(403)
    if newpass1 != newpass2:
        abort(401)
    newpass1 = hash_password(newpass1)
    cur = connection.execute(
        "UPDATE users "
        "SET password = ? "
        "WHERE username = ? ", (newpass1, flask.session['username'],)
    )


@insta485.app.route('/accounts/', methods=['POST'])
def show_target():
    """show/target."""
    operation = flask.request.form['operation']
    if operation == "login":
        login_username = flask.request.form['username']
        login_password = flask.request.form['password']
        do_login(login_username, login_password)
    elif operation == "create":
        login_username = flask.request.form['username']
        login_password = flask.request.form['password']
        fullname = flask.request.form['fullname']
        email = flask.request.form['email']
        fileobj = flask.request.files["file"]
        do_create(login_username, login_password, fullname, email, fileobj)
    elif operation == "delete":
        do_delete()
    elif operation == "edit_account":
        if 'username' not in flask.session:
            abort(403)
        create_fullname = flask.request.form['fullname']
        create_email = flask.request.form['email']
        fileobj = flask.request.files['file']
        if not create_email or not create_fullname:
            abort(400)
        do_edit(create_fullname, create_email, fileobj)
    elif operation == "update_password":
        if 'username' not in flask.session:
            abort(403)
        oldpass = flask.request.form['password']
        newpass1 = flask.request.form['new_password1']
        newpass2 = flask.request.form['new_password2']
        if not oldpass or not newpass1 or not newpass2:
            abort(400)
        do_password(oldpass, newpass1, newpass2)
    if (result := flask.request.args.get("target")):
        return flask.redirect(result)
    return flask.redirect(flask.url_for('show_index'))
