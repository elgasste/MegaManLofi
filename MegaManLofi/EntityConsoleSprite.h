#pragma once

#include <memory>
#include <map>

#include "ConsoleImage.h"
#include "MovementType.h"
#include "Direction.h"

namespace MegaManLofi
{
   class ConsoleSprite;
   class ReadOnlyEntity;

   class EntityConsoleSprite
   {
   public:
      EntityConsoleSprite() { }
      EntityConsoleSprite( EntityConsoleSprite& ecs );

      virtual void AssignTo( const std::shared_ptr<ReadOnlyEntity> entity );
      virtual void AddSprite( MovementType movementType,
                              Direction direction,
                              const std::shared_ptr<ConsoleSprite> sprite );
      virtual void Tick();
      virtual void Reset();

      virtual short GetWidth() const;
      virtual short GetHeight() const;
      virtual float GetTotalTraversalSeconds() const;
      virtual const ConsoleImage& GetCurrentImage() const;

   private:
      std::map<MovementType, std::map<Direction, std::shared_ptr<ConsoleSprite>>> _movementSpriteMaps;

      std::shared_ptr<ReadOnlyEntity> _entity;
   };
}
