import React from 'react';
import './Question.css';

function Question(props) {
  return (
    <div className="Question">
      <h2 className='Feedback'> What do you want to ask or share </h2>

      <label className=''>Title</label>
      <input className='Title' type="text" placeholder="Start your question with how, what, why, etc." />

      <label className='Class'>Describe your Problem</label>
      <textarea className='Description' placeholder="Describe your problem!" />

      <label className='Class'>Tags</label>
      <input className='Tags' type="text" placeholder="Please add up to 3 tags to describe what your question is about, e.g., Java" />

      <button className="post">Post</button>
    </div>
  );
}

export default Question;