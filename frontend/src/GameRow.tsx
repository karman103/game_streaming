import React from 'react';
import GameCard from './GameCard';

type Game = {
  id: string;
  name: string;
  imageUrl: string;
};

type Props = {
  title: string;
  games: Game[];
  onStart: (id: string) => void;
};

const GameRow: React.FC<Props> = ({ title, games, onStart }) => {
  return (
    <div className="my-6">
      <h3 className="text-white text-xl mb-3 ml-4">{title}</h3>
      <div className="flex overflow-x-auto px-4">
        {games.map((game) => (
          <GameCard key={game.id} game={game} onStart={onStart} />
        ))}
      </div>
    </div>
  );
};

export default GameRow;
