import React from "react";
import PropTypes from "prop-types";
import moment from "moment";
import Comment from "./comment";


class Post extends React.Component {
  /* Display image and post owner of a single post
   */
  constructor(props) {
    // Initialize mutable state
    super(props);
    this.like_logic = this.like_logic.bind(this);
    this.like_api_call = this.like_api_call.bind(this);
    this.delete_comment = this.delete_comment.bind(this);
    this.delete_comment_api_call = this.delete_comment_api_call.bind(this);
    this.handleUpdateComments = this.handleUpdateComments.bind(this);
    this.state = {
      imgUrl: "",
      owner: "",
      comments: "",
      likes: "",
      ownerImgUrl: "",
      ownerShowUrl: "",
      postShowUrl: "",
      created: "",
      islike: false,
      postid: 0,
      likeurl: "",
    };
  }

  componentDidMount() {
    // This line automatically assigns this.props.url to the const variable url
    const { url } = this.props;
    console.log(url);
    // Call REST API to get the post's information
    fetch(url, { credentials: "same-origin" })
      .then((response) => {
        if (!response.ok) throw Error(response.statusText);
        return response.json();
      })
      .then((data) => {
        this.setState({
          imgUrl: data.imgUrl,
          owner: data.owner,
          comments: data.comments,
          likes: data.likes.numLikes,
          ownerImgUrl: data.ownerImgUrl,
          ownerShowUrl: data.ownerShowUrl,
          postShowUrl: data.postShowUrl,
          created: data.created,
          islike: data.likes.lognameLikesThis,
          postid: data.postid,
          likeurl: data.likes.url,
        });
      })
      .catch((error) => console.log(error));
  }

  
  
  handleUpdateComments (newcomment) {
    console.log("handleupdate");
    console.log(newcomment);
    this.setState((prevState) => ({
      comments: [...prevState.comments, newcomment],
    }));
  };

  like_logic(props) {
    const { islike, likeurl } = props;
    let text = "like";
    if (islike === true) {
      text = "unlike";
    }
    return (
      <p>
        <button type = "button"
          className="like-unlike-button"
          onClick={() => this.like_api_call(islike, likeurl)}
        >
          {" "}
          {text}{" "}
        </button>
      </p>
    );
  }


  like_api_call(islike, likeurl) {
    console.log(islike);
    console.log(likeurl);
    if (islike) {
      fetch(likeurl, { method: "DELETE" }, { credentials: "same-origin" }).then(
        () => {
          this.setState((prevState) => ({
            islike: !islike,
            likeurl: null,
            likes: prevState.likes - 1,
          }));
        }
      );
    } else {
      fetch(likeurl, { method: "POST" }, { credentials: "same-origin" })
        .then((response) => {
          if (!response.ok) throw Error(response.statusText);
          return response.json();
        })
        .then((data) => {
          this.setState((prevState) => ({
            islike: true,
            likeurl: data.url,
            likes: prevState.likes + 1,
          }));
        })
        .catch((error) => console.log(error));
    }
  }


  delete_comment_api_call(commenturl) {
    fetch(
      commenturl,
      { method: "DELETE" },
      { credentials: "same-origin" }
    ).then(() => {
      this.setState((prevState) => ({
        comments: prevState.comments.filter(
          (onecomment) => onecomment.url !== commenturl
        ),
      }));
    });
  }

  delete_comment(props) {
    const { iscommentowner, commenturl } = props;
    if (iscommentowner === true) {
      return (
        <p>
          <button
            className="delete-comment-button" type = "button"
            onClick={() => this.delete_comment_api_call(commenturl)}
          >
            {" "}
            {"Delete Comment"}{" "}
          </button>
        </p>
      );
    }
    return(null); 
  }

  render() {
    // This line automatically assigns this.state.imgUrl to the const variable imgUrl
    // and this.state.owner to the const variable owner
    const {
      imgUrl,
      owner,
      comments,
      likes,
      ownerImgUrl,
      ownerShowUrl,
      postShowUrl,
      created,
      postid,
      islike,
      likeurl,
    } = this.state;
    // Render post info
    let likeword = " likes";
    if (likes === 1) {
      likeword = " like";
    }
    const humantime = moment(created, "YYYY-MM-DD").fromNow();
    let likeUrl = likeurl;
    if (likeurl === null) {
      likeUrl = `"/api/v1/likes/?postid=" + ${postid}`;
    }

    
    const doubleClick = (event) => {
      if (event.detail === 2 && !islike) {
        this.like_api_call(islike, likeUrl);
      }
    };

    return (
      <div className="post">
        <a href={ownerShowUrl}>
          <img src={ownerImgUrl} alt="owner" />
        </a>
        <a href={ownerShowUrl}>{owner}</a>
        <div> </div>
        <a href={postShowUrl}>{humantime}</a>
        <div> </div>
        <button type = "button" onClick={doubleClick}>
        <img src={imgUrl} alt="post_image" />
        </button>
        <this.like_logic islike={islike} likeurl={likeUrl} handleLikeChange />
        <p>
          {likes}
          {likeword}
        </p>
        {Array.isArray(comments)
          ? comments.map((comment) => (
             
                <div key ={comment}>
                  <a href={comment.ownerShowUrl}>{comment.owner}</a>{" "}
                  {comment.text}
                  <this.delete_comment
                    iscommentowner={comment.lognameOwnsThis}
                    commenturl={comment.url}
                    handleCommentChange
                  />
                </div>
            
          ))
          : null}
        <Comment postid={postid} updateComments={this.handleUpdateComments} />
      </div>
    );
  }
}
Post.propTypes = {
  url: PropTypes.string.isRequired,
};
export default Post;
