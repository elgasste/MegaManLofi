#pragma once

#include <memory>

#include "GameCommandArgs.h"
#include "Coordinate.h"

namespace MegaManLofi
{
   class ReadOnlyEntity;

   class ShootTargetCommandArgs : public GameCommandArgs
   {
   public:
      ShootTargetCommandArgs( const std::shared_ptr<ReadOnlyEntity> sourceEntity,
                              const Coordinate<float>& targetPosition ) :
         SourceEntity( sourceEntity ),
         TargetPosition( targetPosition )
      {
      }

      const std::shared_ptr<ReadOnlyEntity> SourceEntity;
      const Coordinate<float> TargetPosition;
   };
}
