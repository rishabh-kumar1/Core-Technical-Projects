"""REST API for posts."""
import flask
import insta485
from insta485.views.accounts import decrypt_password


class InvalidUsage(Exception):
    """Checking Invalid usage."""

    status_code = 400

    def __init__(self, message, status_code=None, payload=None):
        """Init function."""
        Exception.__init__(self)
        self.message = message
        if status_code is not None:
            self.status_code = status_code
        self.payload = payload

    def to_dict(self):
        """Translate into dictionary."""
        r_v = dict(self.payload or ())
        r_v["message"] = self.message
        r_v["status_code"] = self.status_code
        return r_v


def check_user_auth():
    """Return check."""
    if flask.request.authorization is not None:
        name = flask.request.authorization["username"]
        return name
    if "username" in flask.session:
        name = flask.session["username"]
        return name
    return None


def check_verification():
    """Check verification."""
    connection = insta485.model.get_db()

    if "username" in flask.session:
        return True

    if flask.request.authorization is None:
        raise InvalidUsage("Forbidden", 403)

    username = flask.request.authorization["username"]
    password = flask.request.authorization["password"]

    cur = connection.execute(
        "SELECT username "
        "FROM users "
        "WHERE username = ?", (username,)
    )
    db_result = cur.fetchall()

    if len(db_result) == 0:
        raise InvalidUsage("Forbidden", 403)

    cur = connection.execute(
        "SELECT password "
        "FROM users "
        "WHERE username = ?", (username,)
    )
    db_result = cur.fetchall()
    db_result = db_result[0]["password"]
    if decrypt_password(db_result, password) != db_result:
        raise InvalidUsage("Forbidden", 403)
    return True


@insta485.app.errorhandler(InvalidUsage)
def handle_invalid(error):
    """Handle invalid errors."""
    response = flask.jsonify(error.to_dict())
    response.status_code = error.status_code
    return response


@insta485.app.route("/api/v1/")
def get_services():
    """Get Services."""
    context = {
        "comments": "/api/v1/comments/",
        "likes": "/api/v1/likes/",
        "posts": "/api/v1/posts/",
        "url": "/api/v1/",
    }
    return flask.jsonify(**context)


@insta485.app.route("/api/v1/posts/")
def get_all_posts():
    """Get posts."""
    connection = insta485.model.get_db()
    check_verification()

    username = check_user_auth()

    postid_lte = flask.request.args.get("postid_lte")
    size = flask.request.args.get("size", default=10, type=int)
    page = flask.request.args.get("page", default=0, type=int)

    if size < 0 or page < 0:
        raise InvalidUsage("Bad Request", 400)

    context = {}
    context["next"] = ""
    context["results"] = []
    context["url"] = flask.request.full_path
    if context["url"][-1] == "?":
        context["url"] = context["url"][:-1]
    cur = connection.execute(
        "SELECT username2 "
        "FROM following "
        "WHERE username1 = ? ", (username,)
    )
    db_result = cur.fetchall()
    if len(db_result) == 0:
        return flask.jsonify(**context)

    total_post = []

    cur = connection.execute(
        "SELECT postid "
        "FROM posts "
        "WHERE owner = ? ", (username,)
    )
    all_posts = cur.fetchall()
    for all_p in all_posts:
        if postid_lte is None or (
            postid_lte is not None and all_p["postid"] <= int(postid_lte)
        ):
            total_post.append(all_p["postid"])
    print(total_post)
    for res in db_result:
        cur = connection.execute(
            "SELECT postid "
            "FROM posts "
            "WHERE owner = ? ", (res["username2"],)
        )
        all_posts = cur.fetchall()
        for all_p in all_posts:
            if postid_lte is None or (
                postid_lte is not None and all_p["postid"] <= int(postid_lte)
            ):
                total_post.append(all_p["postid"])

    total_post = sorted(total_post, reverse=True)

    if postid_lte is None:
        postid_lte = total_post[0]

    total_post = total_post[page * size:]

    print(total_post)

    if size <= len(total_post):
        total_post = total_post[0:size]
        a_v = "/api/v1/posts/"
        b_v = postid_lte
        context["next"] = f"{a_v}?size={size}&page={page+1}&postid_lte={b_v}"

    print("hit", total_post)
    for total_p in total_post:
        context["results"].append(
            {"postid": total_p, "url": f"/api/v1/posts/{total_p}/"}
        )

    return flask.jsonify(**context)


@insta485.app.route("/api/v1/likes/", methods=["POST"])  # done i think
def post_add_likes():
    """Add likes to posts."""
    connection = insta485.model.get_db()
    check_verification()

    username = ""

    if flask.request.authorization is not None:
        username = flask.request.authorization["username"]

    elif "username" in flask.session:
        username = flask.session["username"]

    postid = flask.request.args.get("postid")

    context = {}

    cur = connection.execute(
        "SELECT likeid "
        "FROM likes "
        "WHERE owner = ? AND postid = ? ",
        (username, postid),
    )
    liked = cur.fetchall()
    if len(liked) > 0:
        context["likeid"] = liked[0]["likeid"]
        context["url"] = f"/api/v1/likes/{liked[0]['likeid']}/"
        return flask.jsonify(**context), 200

    cursor = connection.cursor()
    cursor.execute(
        "INSERT INTO likes (owner, postid) VALUES (?, ?)", (username, postid)
    )
    connection.commit()

    cur = connection.execute(
        "SELECT likeid "
        "FROM likes "
        "WHERE owner = ? AND postid = ? ",
        (username, postid),
    )
    liked = cur.fetchall()
    context["likeid"] = liked[0]["likeid"]
    context["url"] = f"/api/v1/likes/{liked[0]['likeid']}/"
    return flask.jsonify(**context), 201


@insta485.app.route("/api/v1/likes/<int:likeid>/", methods=["DELETE"])
def post_delete_likes(likeid):
    """Delete likes on posts."""
    connection = insta485.model.get_db()
    check_verification()

    username = ""

    if flask.request.authorization is not None:
        username = flask.request.authorization["username"]

    elif "username" in flask.session:
        username = flask.session["username"]

    context = {}

    cur = connection.execute(
        "SELECT likeid, owner "
        "FROM likes "
        "WHERE likeid = ? ", (likeid,)
    )
    liked = cur.fetchall()
    if len(liked) == 0:
        raise InvalidUsage("not found", 404)
    if liked[0]["owner"] != username:
        raise InvalidUsage("forbidden", 403)

    cursor = connection.cursor()
    cursor.execute("DELETE FROM likes WHERE likeid = ?", (likeid,))
    connection.commit()

    return flask.jsonify(**context), 204


@insta485.app.route("/api/v1/posts/<int:postid>/")
def get_post(postid):
    """Get posts."""
    connection = insta485.model.get_db()
    check_verification()

    username = ""

    if flask.request.authorization is not None:
        username = flask.request.authorization['username']

    elif 'username' in flask.session:
        username = flask.session['username']

    context = {}

    cur = connection.execute(
        "SELECT filename, owner, created "
        "FROM posts "
        "WHERE postid = ? ", (postid,)
    )
    db_result = cur.fetchall()
    if len(db_result) == 0:
        raise InvalidUsage("Not Found", 404)

    context["comments"] = []
    cur = connection.execute(
        "SELECT text, commentid, owner "
        "FROM comments "
        "WHERE postid = ? ", (postid,)
    )
    db_result = cur.fetchall()
    for res in db_result:
        temp = {}
        temp["commentid"] = res["commentid"]
        temp["lognameOwnsThis"] = False
        if res["owner"] == username:
            temp["lognameOwnsThis"] = True
        temp["owner"] = res["owner"]
        temp["ownerShowUrl"] = f"/users/{res['owner']}/"
        temp["text"] = res["text"]
        temp["url"] = f"/api/v1/comments/{res['commentid']}/"
        context["comments"].append(temp)

    context["comments_url"] = f"/api/v1/comments/?postid={postid}"
    cur = connection.execute(
        "SELECT filename, owner, created "
        "FROM posts "
        "WHERE postid = ? ", (postid,)
    )
    db_result = cur.fetchall()
    context["created"] = db_result[0]["created"]
    context["imgUrl"] = f"/uploads/{db_result[0]['filename']}"
    context["owner"] = db_result[0]["owner"]

    cur = connection.execute(
        "SELECT likeid, owner "
        "FROM likes "
        "WHERE postid = ? ", (postid,)
    )
    db_result = cur.fetchall()
    context["likes"] = {}
    context["likes"]["lognameLikesThis"] = False
    context["likes"]["numLikes"] = 0
    context["likes"]["url"] = None
    for res in db_result:
        if res["owner"] == username:
            context["likes"]["lognameLikesThis"] = True
            context["likes"]["url"] = f"/api/v1/likes/{res['likeid']}/"
        context["likes"]["numLikes"] += 1
    cur = connection.execute(
        "SELECT filename "
        "FROM users "
        "WHERE username = ? ", (context["owner"],)
    )
    db_result = cur.fetchall()
    context["ownerImgUrl"] = f"/uploads/{db_result[0]['filename']}"
    context["ownerShowUrl"] = f"/users/{context['owner']}/"
    context["postShowUrl"] = f"/posts/{postid}/"
    context["postid"] = postid
    context["url"] = f"/api/v1/posts/{postid}/"

    return flask.jsonify(**context)


@insta485.app.route("/api/v1/comments/", methods=["POST"])  # NEEDS FIXING
def post_add_comment():
    """Add comments to posts."""
    connection = insta485.model.get_db()
    check_verification()

    username = ""

    if flask.request.authorization is not None:
        username = flask.request.authorization["username"]

    elif "username" in flask.session:
        username = flask.session["username"]

    postid = flask.request.args.get("postid")

    context = {}

    text = flask.request.json.get("text", None)

    cursor = connection.cursor()
    cursor.execute(
        "INSERT INTO comments (owner, postid, text) VALUES (?, ?, ?)",
        (username, postid, flask.request.json.get("text", None)),
    )
    connection.commit()

    cur = connection.execute("SELECT last_insert_rowid() ")
    liked = cur.fetchall()
    context["commentid"] = liked[0]["last_insert_rowid()"]
    context["lognameOwnsThis"] = True
    context["owner"] = username
    context["ownerShowUrl"] = f"/users/{username}/"
    context["text"] = text
    context["url"] = f"api/v1/comments/{context['commentid']}/"
    return flask.jsonify(**context), 201


@insta485.app.route(
    "/api/v1/comments/<int:commentid>/", methods=["DELETE"]
)  # done i think
def post_delete_comments(commentid):
    """Delete comments."""
    connection = insta485.model.get_db()
    check_verification()

    username = ""

    if flask.request.authorization is not None:
        username = flask.request.authorization["username"]

    elif "username" in flask.session:
        username = flask.session["username"]

    context = {}

    cur = connection.execute(
        "SELECT commentid, owner "
        "FROM comments "
        "WHERE commentid = ? ", (commentid,)
    )
    comments = cur.fetchall()
    if len(comments) == 0:
        raise InvalidUsage("not found", 404)
    if comments[0]["owner"] != username:
        raise InvalidUsage("forbidden", 403)

    cursor = connection.cursor()
    cursor.execute("DELETE FROM comments WHERE commentid = ?", (commentid,))
    connection.commit()

    return flask.jsonify(**context), 204
