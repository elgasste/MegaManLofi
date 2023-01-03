#pragma once

#include <memory>

#include "Quad.h"
#include "ArenaTile.h"

namespace MegaManLofi
{
   class IReadOnlyPlayer;
   class IReadOnlyEntity;

   class __declspec( novtable ) IReadOnlyArena
   {
   public:
      virtual const std::shared_ptr<IReadOnlyPlayer> GetPlayer() const = 0;

      virtual const std::shared_ptr<IReadOnlyEntity> GetEntity( int index ) const = 0;
      virtual int GetEntityCount() const = 0;

      virtual float GetWidth() const = 0;
      virtual float GetHeight() const = 0;

      virtual float GetTileWidth() const = 0;
      virtual float GetTileHeight() const = 0;

      virtual int GetHorizontalTiles() const = 0;
      virtual int GetVerticalTiles() const = 0;

      virtual const Quad<float>& GetActiveRegion() const = 0;

      virtual const ArenaTile& GetTile( int index ) const = 0;

      virtual bool HasEntity( int uniqueId ) const = 0;
   };
}
