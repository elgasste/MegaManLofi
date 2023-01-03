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

      void AssignTo( const std::shared_ptr<IReadOnlyEntity> entity ) override;
      void AddSprite( MovementType movementType,
                      Direction direction,
                      const std::shared_ptr<IConsoleSprite> sprite ) override;
      void Tick() override;
      void Reset() override;

      short GetWidth() const override;
      short GetHeight() const override;
      float GetTotalTraversalSeconds() const override;
      const ConsoleImage& GetCurrentImage() const override;

   private:
      std::map<MovementType, std::map<Direction, std::shared_ptr<IConsoleSprite>>> _movementSpriteMaps;

      std::shared_ptr<IReadOnlyEntity> _entity;
   };
}
