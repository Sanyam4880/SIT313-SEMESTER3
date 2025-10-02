import { initializeApp } from "firebase/app";
import { getAuth } from "firebase/auth";
import { getFirestore } from "firebase/firestore";

const firebaseConfig = {
  apiKey: "AIzaSyB6NPBguZBZJTyxmitKW5RAgh2ZRHQgySU",
  authDomain: "t71p-1791b.firebaseapp.com",
  projectId: "t71p-1791b",
  storageBucket: "t71p-1791b.firebasestorage.app",
  messagingSenderId: "740571086881",
  appId: "1:740571086881:web:7e632e95c52571e772e360",
  measurementId: "G-JYVBKESG4M"
};

const app = initializeApp(firebaseConfig);

export const auth = getAuth(app);
export const db = getFirestore(app);
