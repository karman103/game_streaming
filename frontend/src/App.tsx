import React from 'react';
import { BrowserRouter as Router, Routes, Route } from 'react-router-dom';
import GameLauncher from './components/GameLauncher';
import GameStream from './components/GameStream';
import './App.css';

const App: React.FC = () => {
  return (
    <Router>
      <div className="App">
        <Routes>
          <Route path="/" element={<GameLauncher />} />
          <Route 
            path="/session/:sessionId" 
            element={<GameSessionPage />} 
          />
          <Route path="*" element={<NotFoundPage />} />
        </Routes>
      </div>
    </Router>
  );
};

// Game Session Page Component
const GameSessionPage: React.FC = () => {
  const sessionId = window.location.pathname.split('/')[2];
  
  const handleError = (error: string) => {
    console.error('Game stream error:', error);
    // You could show a toast notification here
  };

  const handleStatusChange = (status: string) => {
    console.log('Game status changed:', status);
  };

  return (
    <div className="game-session-page">
      <GameStream 
        sessionId={sessionId}
        onError={handleError}
        onStatusChange={handleStatusChange}
      />
    </div>
  );
};

// 404 Not Found Page
const NotFoundPage: React.FC = () => {
  return (
    <div className="not-found-page">
      <div className="not-found-content">
        <h1>404</h1>
        <h2>Page Not Found</h2>
        <p>The page you're looking for doesn't exist.</p>
        <a href="/" className="home-link">
          ← Back to Home
        </a>
      </div>
    </div>
  );
};

export default App;
