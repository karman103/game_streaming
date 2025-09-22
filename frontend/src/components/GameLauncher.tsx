import React, { useState, useEffect } from 'react';
import './GameLauncher.css';

interface Game {
  id: string;
  name: string;
  description: string;
  image: string;
  port: number;
  type: string;
}

interface Session {
  sessionID: string;
  containerID: string;
  gameType: string;
  port: number;
  status: string;
  createdAt: string;
}

const GameLauncher: React.FC = () => {
  const [games, setGames] = useState<Game[]>([]);
  const [sessions, setSessions] = useState<Session[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [selectedGame, setSelectedGame] = useState<Game | null>(null);
  const [isStarting, setIsStarting] = useState(false);

  // Fetch available games
  useEffect(() => {
    fetchGames();
    fetchSessions();
  }, []);

  const fetchGames = async () => {
    try {
      const response = await fetch('/api/games');
      if (!response.ok) throw new Error('Failed to fetch games');
      const data = await response.json();
      setGames(data.games);
    } catch (err) {
      setError(err instanceof Error ? err.message : 'Failed to fetch games');
    }
  };

  const fetchSessions = async () => {
    try {
      const response = await fetch('/api/sessions');
      if (!response.ok) throw new Error('Failed to fetch sessions');
      const data = await response.json();
      setSessions(data.sessions);
    } catch (err) {
      console.error('Failed to fetch sessions:', err);
    } finally {
      setLoading(false);
    }
  };

  const startGame = async (game: Game) => {
    setIsStarting(true);
    setError(null);

    try {
      const response = await fetch('/api/sessions', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          gameType: game.id,
        }),
      });

      if (!response.ok) {
        const errorData = await response.json();
        throw new Error(errorData.error || 'Failed to start game');
      }

      const session = await response.json();
      
      // Redirect to game session
      window.location.href = `/session/${session.sessionID}`;
    } catch (err) {
      setError(err instanceof Error ? err.message : 'Failed to start game');
    } finally {
      setIsStarting(false);
    }
  };

  const stopSession = async (sessionID: string) => {
    try {
      const response = await fetch(`/api/sessions/${sessionID}`, {
        method: 'DELETE',
      });

      if (!response.ok) {
        const errorData = await response.json();
        throw new Error(errorData.error || 'Failed to stop session');
      }

      // Refresh sessions list
      fetchSessions();
    } catch (err) {
      setError(err instanceof Error ? err.message : 'Failed to stop session');
    }
  };

  const joinSession = (sessionID: string) => {
    window.location.href = `/session/${sessionID}`;
  };

  if (loading) {
    return (
      <div className="game-launcher-loading">
        <div className="loading-spinner"></div>
        <p>Loading games...</p>
      </div>
    );
  }

  return (
    <div className="game-launcher">
      <div className="launcher-header">
        <h1>🎮 Game Streaming Platform</h1>
        <p>Play games in the cloud with low-latency streaming</p>
      </div>

      {error && (
        <div className="error-banner">
          <span className="error-icon">⚠️</span>
          <span>{error}</span>
          <button onClick={() => setError(null)}>×</button>
        </div>
      )}

      <div className="launcher-content">
        {/* Available Games */}
        <section className="games-section">
          <h2>Available Games</h2>
          <div className="games-grid">
            {games.map((game) => (
              <div key={game.id} className="game-card">
                <div className="game-image">
                  <div className="game-placeholder">
                    {game.name.charAt(0)}
                  </div>
                </div>
                <div className="game-info">
                  <h3>{game.name}</h3>
                  <p>{game.description}</p>
                  <div className="game-meta">
                    <span className="game-type">{game.type}</span>
                    <span className="game-port">Port: {game.port}</span>
                  </div>
                </div>
                <div className="game-actions">
                  <button
                    className="start-button"
                    onClick={() => startGame(game)}
                    disabled={isStarting}
                  >
                    {isStarting ? 'Starting...' : 'Start Game'}
                  </button>
                </div>
              </div>
            ))}
          </div>
        </section>

        {/* Active Sessions */}
        {sessions.length > 0 && (
          <section className="sessions-section">
            <h2>Active Sessions</h2>
            <div className="sessions-list">
              {sessions.map((session) => (
                <div key={session.sessionID} className="session-card">
                  <div className="session-info">
                    <h4>{session.gameType}</h4>
                    <p>Session: {session.sessionID}</p>
                    <div className="session-meta">
                      <span className={`status-badge ${session.status}`}>
                        {session.status}
                      </span>
                      <span className="session-port">Port: {session.port}</span>
                    </div>
                  </div>
                  <div className="session-actions">
                    <button
                      className="join-button"
                      onClick={() => joinSession(session.sessionID)}
                    >
                      Join
                    </button>
                    <button
                      className="stop-button"
                      onClick={() => stopSession(session.sessionID)}
                    >
                      Stop
                    </button>
                  </div>
                </div>
              ))}
            </div>
          </section>
        )}
      </div>

      <div className="launcher-footer">
        <div className="footer-info">
          <p>Powered by Docker • WebRTC Streaming • Low Latency</p>
        </div>
        <div className="footer-links">
          <a href="/health" target="_blank">Health Check</a>
          <a href="/api/games" target="_blank">API</a>
        </div>
      </div>
    </div>
  );
};

export default GameLauncher;
