import "./Articles.css";

function Articles() {
  const articles = [
    { title: "Discrete Mathematics", desc: "Learn Discrete Mathematics", author: "Discrete Math Simplified", img: "discretemathematics.jpg" },
    { title: "Embedded System", desc: "Learn Embedded System", author: "Embedded Bytes",  img: "embeddedsystem.jpg" },
    { title: "C++", desc: "Learn C++", author: "Simple C++", img: "c++.jpg" },
  ];

  return (
    <div className="articles">
      <h1>Featured Articles</h1>

      <div className="box">
        {articles.map((article, index) => (
          <div className="card">
            <img src={article.img} alt={article.title} />
            <h2>{article.title}</h2>
            <p>{article.desc}</p>
            <p>-----------------------</p>
            <p>⭐{article.author}</p>
          </div>
        ))}
      </div>
      <button>See all articles</button>
    </div>
  );
}

export default Articles;
