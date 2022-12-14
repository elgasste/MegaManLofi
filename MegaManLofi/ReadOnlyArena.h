#pragma once

#include <memory>
#include <vector>

#include "Rectangle.h"
#include "ArenaTile.h"
#include "SpawnPoint.h"

namespace MegaManLofi
{
   class Entity;
   class ReadOnlyEntity;

   class ReadOnlyArena
   {
   public:
      ReadOnlyArena();

      virtual int GetArenaId() const { return _arenaId; }
      virtual const std::shared_ptr<ReadOnlyEntity> GetEntity( int index ) const;
      virtual const std::shared_ptr<ReadOnlyEntity> GetPlayerEntity() const;
      virtual int GetEntityCount() const { return (int)_entities.size(); }
      virtual float GetWidth() const { return _width; }
      virtual float GetHeight() const { return _height; }
      virtual int GetHorizontalTiles() const { return _horizontalTiles; }
      virtual int GetVerticalTiles() const { return _verticalTiles; }
      virtual const ArenaTile& GetTile( int index ) const { return _tiles[index]; }
      virtual bool HasEntity( int uniqueId ) const;

   protected:
      int _arenaId;
      std::vector<std::shared_ptr<Entity>> _entities;
      std::shared_ptr<Entity> _playerEntity;
      std::vector<ArenaTile> _tiles;
      std::vector<std::shared_ptr<SpawnPoint>> _spawnPoints;
      float _width;
      float _height;
      int _horizontalTiles;
      int _verticalTiles;
      Rectangle<float> _activeRegion;
      Rectangle<float> _deSpawnRegion;
   };
}
