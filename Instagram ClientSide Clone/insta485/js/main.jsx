import React from "react";
import { createRoot } from "react-dom/client";
import InfiniteScroll from "react-infinite-scroll-component";
import Post from "./post";

// Create a root
// This method is only called once
// Insert the post component into the DOM

class App extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
    hasMore: true,
    posts: new [],
    nextUrl: "/api/v1/posts/",
    }
  };

  componentDidMount() {
    const {nextUrl } = this.state
    fetch(nextUrl, { credentials: "same-origin" })
      .then((response) => {
        if (!response.ok) throw Error(response.statusText);
        return response.json();
      })
      .then((data) => {
        this.setState({
          posts: data.results,
          nextUrl: data.next,
        });
      })
      .catch((error) => console.log(error));
  }

  fetchMoreData () {
    const { nextUrl } = this.state
    console.log(nextUrl);
    if (nextUrl === "") {
      this.setState({
        hasMore: false,
      });
      return;
    }
    fetch(nextUrl, { credentials: "same-origin" })
      .then((response) => {
        if (!response.ok) throw Error(response.statusText);
        return response.json();
      })
      .then((data) => {
        this.setState((prevState) => ({
          posts: prevState.posts.concat(data.results),
          nextUrl: data.next,
        }));
      })
      .catch((error) => console.log(error));
  };

  render() {
    const { posts } = this.state
    const { hasMore } = this.state
    console.log(posts);
    return (
      <div>
        <hr />
        <InfiniteScroll
          dataLength={posts.length}
          next={this.fetchMoreData}
          hasMore={hasMore}
          loader={<h4>Loading....</h4>}
          endMessage={
            <p style={{ textAlign: "center" }}>
              <b>Yay! You have seen it all</b>
            </p>
          }
        >
          {posts.map((i) => (
            <div key={i}>
              <Post url={i.url} />
            </div>
          ))}
        </InfiniteScroll>
      </div>
    );
  }
}

const root = createRoot(document.getElementById("reactEntry"));
root.render(<App />);
