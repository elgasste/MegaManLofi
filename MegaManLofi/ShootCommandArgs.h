#pragma once

#include <memory>

#include "GameCommandArgs.h"

namespace MegaManLofi
{
   class ReadOnlyEntity;

   class ShootCommandArgs : public GameCommandArgs
   {
   public:
      ShootCommandArgs( const std::shared_ptr<ReadOnlyEntity> sourceEntity ) :
         _sourceEntity( sourceEntity )
      {
      }

      const std::shared_ptr<ReadOnlyEntity> _sourceEntity;
   };
}
