#pragma once

#include <memory>

#include "ConsoleImage.h"
#include "MovementType.h"
#include "Direction.h"

namespace MegaManLofi
{
   class ConsoleSprite;
   class ReadOnlyEntity;

   class __declspec( novtable ) IEntityConsoleSprite
   {
   public:
      virtual void AssignTo( const std::shared_ptr<ReadOnlyEntity> entity ) = 0;
      virtual void AddSprite( MovementType movementType,
                              Direction direction,
                              const std::shared_ptr<ConsoleSprite> sprite ) = 0;
      virtual void Tick() = 0;
      virtual void Reset() = 0;

      virtual short GetWidth() const = 0;
      virtual short GetHeight() const = 0;
      virtual float GetTotalTraversalSeconds() const = 0;
      virtual const ConsoleImage& GetCurrentImage() const = 0;
   };
}
