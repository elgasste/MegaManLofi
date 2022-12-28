#include "EntityConsoleSprite.h"
#include "ConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

EntityConsoleSprite::EntityConsoleSprite() :
   _currentMovementType( (MovementType)0 ),
   _currentDirection( (Direction)0 )
{
}

EntityConsoleSprite::EntityConsoleSprite( EntityConsoleSprite& ecs ) :
   _currentMovementType( ecs._currentMovementType ),
   _currentDirection( ecs._currentDirection )
{
   for ( auto [movementType, directionSpriteMap] : ecs._movementSpriteMaps )
   {
      for ( auto [direction, sprite] : directionSpriteMap )
      {
         auto realSprite = static_pointer_cast<ConsoleSprite>( sprite );
         _movementSpriteMaps[movementType][direction] = make_shared<ConsoleSprite>( *realSprite );
      }
   }
}

void EntityConsoleSprite::AddSprite( MovementType movementType,
                                     Direction direction,
                                     const std::shared_ptr<IConsoleSprite> sprite )
{
   _movementSpriteMaps[movementType][direction] = sprite;
}

void EntityConsoleSprite::Tick()
{
   _movementSpriteMaps[_currentMovementType][_currentDirection]->Tick();
}

void EntityConsoleSprite::Reset()
{
   for ( auto [movementType, directionSpriteMap] : _movementSpriteMaps )
   {
      for ( auto [direction, sprite] : directionSpriteMap )
      {
         sprite->Reset();
      }
   }
}

short EntityConsoleSprite::GetWidth() const
{
   return _movementSpriteMaps.at( _currentMovementType ).at( _currentDirection )->GetWidth();
}

short EntityConsoleSprite::GetHeight() const
{
   return _movementSpriteMaps.at( _currentMovementType ).at( _currentDirection )->GetHeight();
}

double EntityConsoleSprite::GetTotalTraversalSeconds() const
{
   return _movementSpriteMaps.at( _currentMovementType ).at( _currentDirection )->GetTotalTraversalSeconds();
}

const ConsoleImage& EntityConsoleSprite::GetCurrentImage() const
{
   return _movementSpriteMaps.at( _currentMovementType ).at( _currentDirection )->GetCurrentImage();
}
