#include "EntityConsoleSprite.h"
#include "ConsoleSprite.h"
#include "IReadOnlyEntity.h"

using namespace std;
using namespace MegaManLofi;

EntityConsoleSprite::EntityConsoleSprite() :
   _entity( nullptr )
{
}

EntityConsoleSprite::EntityConsoleSprite( EntityConsoleSprite& ecs ) :
   _entity( ecs._entity )
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

void EntityConsoleSprite::AssignTo( const shared_ptr<IReadOnlyEntity> entity )
{
   _entity = entity;
   Reset();
}

void EntityConsoleSprite::AddSprite( MovementType movementType,
                                     Direction direction,
                                     const std::shared_ptr<IConsoleSprite> sprite )
{
   _movementSpriteMaps[movementType][direction] = sprite;
}

void EntityConsoleSprite::Tick()
{
   _movementSpriteMaps[_entity->GetMovementType()][_entity->GetDirection()]->Tick();
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
   return _movementSpriteMaps.at( _entity->GetMovementType() ).at( _entity->GetDirection() )->GetWidth();
}

short EntityConsoleSprite::GetHeight() const
{
   return _movementSpriteMaps.at( _entity->GetMovementType() ).at( _entity->GetDirection() )->GetHeight();
}

float EntityConsoleSprite::GetTotalTraversalSeconds() const
{
   return _movementSpriteMaps.at( _entity->GetMovementType() ).at( _entity->GetDirection() )->GetTotalTraversalSeconds();
}

const ConsoleImage& EntityConsoleSprite::GetCurrentImage() const
{
   return _movementSpriteMaps.at( _entity->GetMovementType() ).at( _entity->GetDirection() )->GetCurrentImage();
}
