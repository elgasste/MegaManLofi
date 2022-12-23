#pragma once

#include <memory>

#include "ArenaTile.h"

namespace MegaManLofi
{
   class IPlayer;

   class __declspec( novtable ) IArena
   {
   public:
      virtual void Reset() = 0;

      virtual const std::shared_ptr<IPlayer> GetPlayer() const = 0;
      virtual void SetPlayer( const std::shared_ptr<IPlayer> player ) = 0;

      virtual long long GetWidth() const = 0;
      virtual long long GetHeight() const = 0;

      virtual long long GetTileWidth() const = 0;
      virtual long long GetTileHeight() const = 0;

      virtual int GetHorizontalTiles() const = 0;
      virtual int GetVerticalTiles() const = 0;

      virtual const ArenaTile& GetTile( long long index ) const = 0;
   };
}
