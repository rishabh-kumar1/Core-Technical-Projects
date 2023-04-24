import React from "react";
import PropTypes from "prop-types";

class Comment extends React.Component {
  constructor(props) {
    super(props);
    this.state = { value: "" };

    this.handleChange = this.handleChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
  }

  handleChange(event) {
    this.setState({ value: event.target.value });
  }

  handleSubmit(event) {
    const { postid, updateComments } = this.props;
    this.url = `"/api/v1/comments/?postid=" + ${postid}`;
    const { value } = this.state
    const data = { text: value};

    fetch(
      this.url,
      {
        method: "POST",
        headers: {
          Accept: "application/json",
          "Content-Type": "application/json",
        },
        body: JSON.stringify(data),
      },
      { credentials: "same-origin" }
    )
      .then((response) => {
        if (!response.ok) throw Error(response.statusText);
        return response.json();
      })
      .then((data1) => {
        const { props } = this
        props.updateComments(data1);
        console.log(updateComments);
      })
      .catch((error) => console.log(error));

    event.preventDefault();
    this.setState({
      value: "",
    });
  }

  render() {
    const { value } = this.state
    return (
      <form className="comment-form" onSubmit={this.handleSubmit}>
        <input
          type="text"
          placeholder="Enter comment here..."
          value={value}
          onChange={this.handleChange}
        />
      </form>
    );
  }
}
Comment.propTypes = {
  postid: PropTypes.number.isRequired,
  updateComments: PropTypes.func.isRequired,
};
export default Comment;
