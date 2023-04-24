import React from "react";
import PropTypes from "prop-types";

class Like extends React.Component {
  /* Display comment text
   */
  constructor(props) {
    // Initialize mutable state
    super(props);
    this.state = { text: "" };
  }

  componentDidMount() {
    // This line automatically assigns this.props.url to the const variable url
    const { url, lognameLikesThis } = this.props;
    // const { islike } = this.props.lognameLikesThis;
    console.log(lognameLikesThis);
    console.log(this.url);
    // Call REST API to get the post's information
    if (lognameLikesThis) {
      // delete
      console.log("asdasdasda");
      fetch(url, { credentials: "same-origin" })
        .then((response) => {
          if (!response.ok) throw Error(response.statusText);
          return response.json();
        })
        .catch((error) => console.log(error));
    } else {
      console.log("wwwwwwwwwwwwww");
      fetch(url, { credentials: "same-origin" })
        .then((response) => {
          if (!response.ok) throw Error(response.statusText);
          return response.json();
        })
        .catch((error) => console.log(error));
    }
  }

  like_logic() {
    const { text } = this.state
    const { lognameLikesThis } = this.state
    console.log(lognameLikesThis);
    if (text === "like") {
      // like stuff
      console.log("hello");
      this.state.text = "unlike";
      this.state.lognameLikesThis = false;
    } else {
      // unlike stuff
      console.log("goodbye");
      this.state.text = "like";
      this.state.lognameLikesThis = true;
    }
  }

  render() {
    // This line automatically assigns this.state.imgUrl to the const variable imgUrl
    // and this.state.owner to the const variable owner
    const { lognameLikesThis } = this.state;
    // Render comment
    this.state.text = "like";
    if (lognameLikesThis === true) {
      this.state.text = "unlike";
    }
    return (
      <div className="like">
        <p>
          <button type = "button" onClick={() => this.like_logic()}> {this.text} </button>
        </p>
      </div>
    );
  }
}
Like.propTypes = {
  lognameLikesThis: PropTypes.bool.isRequired,
  url: PropTypes.string.isRequired,
};
export default Like;
