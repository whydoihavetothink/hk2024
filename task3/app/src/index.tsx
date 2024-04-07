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
    <App />
    <Comp/>
    <Morse/>
  </React.StrictMode>
);
