import "./Navbar.css";

function Navbar() {
  return (
    <nav>
      <h3>DEV@Deakin</h3>
      <input type="text" placeholder="Search" />
      <div>
        <button>Post</button>
        <button>Login</button>
      </div>
    </nav>
  );
}

export default Navbar;
