import React, { useEffect, useState } from 'react';
import GameRow from '../components/GameRow';
import { useNavigate } from "react-router-dom";

type Game = {
  id: string;
  name: string;
  imageUrl: string;
};

const HomePage: React.FC = () => {
  const [games, setGames] = useState<Game[]>([]);
    const navigate = useNavigate();


  useEffect(() => {
    // TEMPORARY MOCK DATA
    const mockGames: Game[] = [
      {
        id: "1",
        name: "Civilization",
        imageUrl: "https://assets.nintendo.com/image/upload/c_fill,w_1200/q_auto:best/f_auto/dpr_2.0/ncom/software/switch/70010000013704/918c0badde3aeba760e2185f382a2402248a1292322cf540fd8d098eeb292e1e"
      },
      {
        id: "2",
        name: "Mario",
        imageUrl: "https://www.nintendo.com/eu/media/images/10_share_images/portals_3/2x1_SuperMarioHub_image1600w.jpg"
      },
      {
        id: "3",
        name: "God of War",
        imageUrl: "https://via.placeholder.com/200x300?text=God+of+War"
      }

    ];

    setGames(mockGames);
  }, []);


  const handleStartSession = async (gameId: string) => {
    console.log("Starting session for game:", gameId);
    try {
      // const res = await fetch("/api/sessions", { method: "POST" });
      // const data = await res.json();

      navigate(`/game/${gameId}`);
    } catch (err) {
      console.error(err);
    }
};


  return (
    <div className="bg-black min-h-screen text-white">
      <header className="p-6 text-3xl font-bold">🎮 CloudGaming</header>
      <GameRow title="Most Played" games={games} onStart={handleStartSession} />
      <GameRow title="New Releases" games={games} onStart={handleStartSession} />
      <footer className="p-4 text-center text-gray-400">Powered by WebRTC & React</footer>
    </div>
  );
};

export default HomePage;
