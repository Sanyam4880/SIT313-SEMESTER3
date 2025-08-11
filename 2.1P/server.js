import express from 'express';
import bodyParser from 'body-parser';
import path from 'path';
import sgMail from '@sendgrid/mail';
import dotenv from 'dotenv';
import { fileURLToPath } from 'url';

// Needed to get __dirname in ES modules
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

// Load environment variables
dotenv.config();

const app = express();
const PORT = process.env.PORT || 3000;

sgMail.setApiKey(process.env.SENDGRID_API_KEY);

app.use(bodyParser.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname, 'public')));

app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'views', 'index.html'));
});

app.post('/subscribe', (req, res) => {
  const { email } = req.body;

  const msg = {
    to: email,
    from: process.env.FROM_EMAIL,
    subject: 'Welcome to Dev@Deakin!',
    html: `<h1>Welcome to Dev@Deakin</h1><p>We’re happy you joined us!</p>`,
  };

  sgMail
    .send(msg)
    .then(() => {
      res.send(`<h2>Email sent successfully to ${email}</h2><a href="/">Back</a>`);
    })
    .catch((error) => {
      console.error(error);
      res.send(`<h2>Failed to send email. Check console.</h2><a href="/">Back</a>`);
    });
});

app.listen(PORT, () => {
  console.log(`Server running at http://localhost:${PORT}`);
});
