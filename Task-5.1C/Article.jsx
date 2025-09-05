import React from 'react';
import './Article.css';

function Article() {
  return (
    <div className="Article">
      <h2 className="Feedback">What do you want to ask or share</h2>

      <label>Title</label>
      <input type="text" placeholder="Enter a descriptive title." />

      <label>Abstract</label>
      <textarea placeholder="Enter a 1-paragraph Abstract" />

      <label>Article Text</label>
      <textarea placeholder="Enter the main text of the article" />

      <label>Tags</label>
      <input type="text" placeholder="Please add up to three tags to describe what your article is about, e.g., Java" />

      <button className="post">Post</button>
    </div>
  );
}

export default Article;