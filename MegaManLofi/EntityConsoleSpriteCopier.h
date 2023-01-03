#pragma once

#include <memory>

namespace MegaManLofi
{
   class EntityConsoleSprite;

   class EntityConsoleSpriteCopier
   {
   public:
      virtual const std::shared_ptr<EntityConsoleSprite> MakeCopy( const std::shared_ptr<EntityConsoleSprite> sprite ) const;
   };
}
