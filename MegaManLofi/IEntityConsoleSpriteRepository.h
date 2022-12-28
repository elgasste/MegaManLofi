#pragma once

#include <memory>

namespace MegaManLofi
{
   class IEntityConsoleSprite;

   class __declspec( novtable ) IEntityConsoleSpriteRepository
   {
   public:
      virtual const std::shared_ptr<IEntityConsoleSprite> GetSprite( int uniqueId ) const = 0;
   };
}
