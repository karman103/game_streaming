import React from 'react';
import './GameCard.css'; // Assuming you have a CSS file for styles
type Game = {
  id: string;
  name: string;
  imageUrl: string;
};

type Props = {
  game: Game;
  onStart: (id: string) => void;
};

const GameCard: React.FC<Props> = ({ game, onStart }) => {
  return (
    <div className="game-card">
      <img
  src={game.imageUrl}
  alt={game.name}
    className="game-image"
/>

      <div className="p-4">
        <h2 className="text-white text-lg">{game.name}</h2>
        <button
          onClick={() => onStart(game.id)}
          className="mt-2 bg-red-500 text-white px-4 py-2 rounded hover:bg-red-600"
        >
          Play
        </button>
      </div>
    </div>
  );
};

export default GameCard;
