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

      virtual long long GetWidth() const = 0;
      virtual long long GetHeight() const = 0;

      virtual long long GetTileWidth() const = 0;
      virtual long long GetTileHeight() const = 0;

      virtual int GetHorizontalTiles() const = 0;
      virtual int GetVerticalTiles() const = 0;

      virtual const Quad<long long>& GetActiveRegion() const = 0;

      virtual const ArenaTile& GetTile( long long index ) const = 0;
   };
}
