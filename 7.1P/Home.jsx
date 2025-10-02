import { auth } from "../firebase.js";
import { signOut } from "firebase/auth";
import { useNavigate } from "react-router-dom";
import Navbar from "../components/Navbar";
import "./Home.css"; // import the CSS

export default function Home() {
  const navigate = useNavigate();

  const handleLogout = async () => {
    await signOut(auth);
    navigate("/");
  };

  return (
    <div className="home-container">
      <Navbar />
      <h2>Welcome to DEV@Deakin</h2>
      <p>You are successfully logged in.</p>
      <button onClick={handleLogout}>Logout</button>
    </div>
  );
}
