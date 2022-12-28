#pragma once

#include <map>

#include "IEntityConsoleSprite.h"

namespace MegaManLofi
{
   class EntityConsoleSprite : public IEntityConsoleSprite
   {
   public:
      EntityConsoleSprite();
      EntityConsoleSprite( EntityConsoleSprite& ecs );

      void AddSprite( MovementType movementType,
                      Direction direction,
                      const std::shared_ptr<IConsoleSprite> sprite ) override;
      void Tick() override;
      void Reset() override;

      short GetWidth() const override;
      short GetHeight() const override;
      double GetTotalTraversalSeconds() const override;
      const ConsoleImage& GetCurrentImage() const override;

      void SetMovementType( MovementType type ) override { _currentMovementType = type; }
      void SetDirection( Direction direction ) override { _currentDirection = direction; }

   private:
      std::map<MovementType, std::map<Direction, std::shared_ptr<IConsoleSprite>>> _movementSpriteMaps;

      MovementType _currentMovementType;
      Direction _currentDirection;
   };
}
