#pragma once

#include "IEntityConsoleSpriteCopier.h"

namespace MegaManLofi
{
   class EntityConsoleSpriteCopier : public IEntityConsoleSpriteCopier
   {
   public:
      const std::shared_ptr<IEntityConsoleSprite> MakeCopy( const std::shared_ptr<IEntityConsoleSprite> sprite ) const override;
   };
}
