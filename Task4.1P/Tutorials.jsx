import "./Tutorials.css";

function Tutorials() {
  const tutorials = [
    { title: "JavaScript Tutorial", desc: "Learn JavaScript", username: "Code with JavaScript", img: "Javascript.jpg" },
    { title: "Python Tutorial", desc: "Learn Python", username: "Python Made Easy",img: "python.jpg" },
    { title: "CSS Tutorial", desc: "Learn CSS", username: "Style with CSS" , img: "css.jpg" },
  ];

  return (
    <div className="tutors">
      <h1>Featured Tutorials</h1>

      <div className="box">
        {tutorials.map((tut, index) => (
          <div className="cards">
            <img src={tut.img} alt={tut.title} />
            <h2>{tut.title}</h2>
            <p>{tut.desc}</p>
            <p>-----------------------</p>
            <p>⭐{tut.username}</p>
          </div>
        ))}
      </div>
      <button>See all tutorials</button>
    </div>
  );
}

export default Tutorials;

