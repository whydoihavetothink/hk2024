import React from 'react';
import ReactDOM from 'react-dom/client';
import './index.css';
import App from './App';
import Comp from './Comp';
import Morse from './Morse';

const root = ReactDOM.createRoot(
  document.getElementById('root') as HTMLElement
);
root.render(
  <React.StrictMode>
    <h1>Upload MP3</h1>
    <App />
    <h1>Composer</h1>
    <Comp />
    <h1>Morse</h1>
    <Morse/>
  </React.StrictMode>
);
