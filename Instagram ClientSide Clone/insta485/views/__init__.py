"""Views, one for each Insta485 page."""
from insta485.views.index import show_index
from insta485.views.accounts import show_create, show_delete, show_edit
from insta485.views.accounts import show_login_get, show_logout, show_password
from insta485.views.accounts import show_target
from insta485.views.users import get_users_slug, get_users_followers
from insta485.views.users import get_users_following, show_following
from insta485.views.users import show_comments, show_likes, show_posts
