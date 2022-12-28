#pragma once

#include <memory>

namespace MegaManLofi
{
   class IEntityConsoleSprite;

   class __declspec( novtable ) IEntityConsoleSpriteCopier
   {
   public:
      virtual const std::shared_ptr<IEntityConsoleSprite> MakeCopy( const std::shared_ptr<IEntityConsoleSprite> sprite ) const = 0;
   };
}
