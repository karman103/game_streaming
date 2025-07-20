import React from 'react';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import HomePage from './Pages/HomePage';
import SessionPage from './SessionPage';
import GamePage from './Pages/GamePage';

const App: React.FC = () => {
  return (
    <Router>
      <Routes>
        <Route path="/" element={<HomePage />} />
        <Route path="/session/:sessionId" element={<SessionPage />} />
        <Route path="/game/test" element={<GamePage />} />
        <Route path="/game/:gameId" element={<GamePage />} />
        {/* Add more routes as needed */}
        <Route path="*" element={<div>404 Not Found</div>} />
      </Routes>
    </Router>
  );
};

export default App;
