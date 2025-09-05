import React, { useState } from 'react';
import Question from './Question';
import ArticleSection from './Article';

function Selection({ defaultValue = 'Question' }) {
  const [value, setValue] = useState(defaultValue);

  return (
    <div>
      <p>Selected value: <b>{value}</b></p>

      <label>
        <input
          type="radio"
          name="option"
          value="Question"
          checked={value === 'Question'}
          onChange={(e) => setValue(e.target.value)}
        />
        Question
      </label>

      <label style={{ marginLeft: '10px' }}>
        <input
          type="radio"
          name="option"
          value="Article"
          checked={value === 'Article'}
          onChange={(e) => setValue(e.target.value)}
        />
        Article
      </label>

      <div style={{ marginTop: '20px' }}>
        {value === 'Question' ? <Question /> : <ArticleSection />}
      </div>
    </div>
  );
}

export default Selection;