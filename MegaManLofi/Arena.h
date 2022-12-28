#pragma once

#include <vector>

#include "IArena.h"

namespace MegaManLofi
{
   class ArenaDefs;
   class IGameEventAggregator;

   class Arena : public IArena
   {
   public:
      Arena( const std::shared_ptr<ArenaDefs> arenaDefs,
             const std::shared_ptr<IGameEventAggregator> eventAggregator );

      void Reset();

      const std::shared_ptr<IReadOnlyPlayer> GetPlayer() const;
      const std::shared_ptr<IPlayer> GetMutablePlayer() const { return _player; }
      void SetPlayer( const std::shared_ptr<IPlayer> player ) override;

      long long GetWidth() const override { return _width; }
      long long GetHeight() const override { return _height; }

      long long GetTileWidth() const override { return _tileWidth; }
      long long GetTileHeight() const override { return _tileHeight; }

      int GetHorizontalTiles() const override { return _horizontalTiles; }
      int GetVerticalTiles() const override { return _verticalTiles; }

      const Quad<long long>& GetActiveRegion() const override { return _activeRegion; }
      void SetActiveRegion( Quad<long long> region ) override { _activeRegion = region; }

      const ArenaTile& GetTile( long long index ) const override { return _tiles[index]; }

      bool HasEntity( int uniqueId ) const override;

      void AddEntity( const std::shared_ptr<IEntity> entity ) override;
      void RemoveEntity( const std::shared_ptr<IEntity> entity ) override;
      const std::shared_ptr<IReadOnlyEntity> GetEntity( int index ) const override;
      const std::shared_ptr<IEntity> GetMutableEntity( int index ) const override { return _entities[index]; }
      int GetEntityCount() const override { return (int)_entities.size(); }

   private:
      const std::shared_ptr<ArenaDefs> _arenaDefs;
      const std::shared_ptr<IGameEventAggregator> _eventAggregator;

      std::shared_ptr<IPlayer> _player;

      std::vector<ArenaTile> _tiles;
      std::vector<std::shared_ptr<IEntity>> _entities;

      long long _width;
      long long _height;

      long long _playerPositionX;
      long long _playerPositionY;

      long long _tileWidth;
      long long _tileHeight;

      int _horizontalTiles;
      int _verticalTiles;

      Quad<long long> _activeRegion;
   };
}
