import "./Footer.css";

function Footer() {
  return (
    <footer>
      <div className="linking">
        <div>
          <h4>Explore</h4>
          <ul>
            <li>Home</li>
            <li>Questions</li>
            <li>Articles</li>
            <li>Tutorials</li>
          </ul>
        </div>

        <div>
          <h4>Support</h4>
          <ul>
            <li>FAQs</li>
            <li>Help</li>
            <li>Contact Us</li>
          </ul>
        </div>

        <div>
          <h4>Stay connected</h4>
          <div className="app">
            <img src="oip.jpg" alt="Instagram" />
            <img src="facebook.jpg" alt="Facebook" />
            <img src="twitter.jpg" alt="Twitter" />
          </div>
        </div>
      </div>

      <div className="bott">
        <div>DEV@Deakin 2025</div>
        <ul>
          <li>Privacy Policy</li>
          <li>Terms</li>
          <li>Code of Conduct</li>
        </ul>
      </div>
    </footer>
  );
}

export default Footer;
