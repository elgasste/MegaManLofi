#include "EntityPhysics.h"
#include "IFrameRateProvider.h"
#include "FrameActionRegistry.h"
#include "Stage.h"
#include "Arena.h"
#include "Entity.h"

using namespace std;
using namespace MegaManLofi;

EntityPhysics::EntityPhysics( const shared_ptr<IFrameRateProvider> frameRateProvider,
                              const shared_ptr<FrameActionRegistry> frameActionRegistry ) :
   _frameRateProvider( frameRateProvider ),
   _frameActionRegistry( frameActionRegistry ),
   _stage( nullptr )
{
}

void EntityPhysics::AssignTo( const shared_ptr<Stage> stage )
{
   _stage = stage;
}

void EntityPhysics::Tick()
{
   auto arena = _stage->GetMutableActiveArena();

   for ( int i = 0; i < arena->GetEntityCount(); i++ )
   {
      auto entity = arena->GetMutableEntity( i );
      ApplyGravity( entity );
      ApplyFriction( entity );
   }
}

void EntityPhysics::ApplyGravity( const shared_ptr<Entity> entity )
{
   auto currentVelocityY = entity->GetVelocityY();
   auto gravityVelocityDelta = entity->GetGravityAccelerationPerSecond() * _frameRateProvider->GetFrameSeconds();
   auto entityType = entity->GetEntityType();

   if ( entityType == EntityType::Player || entityType == EntityType::Enemy || entityType == EntityType::Item )
   {
      float newVelocityY;

      if ( currentVelocityY < 0 && entityType == EntityType::Player && !_frameActionRegistry->ActionFlagged( FrameAction::PlayerJumping ) )
      {
         newVelocityY = 0;
      }
      else
      {
         newVelocityY = currentVelocityY + gravityVelocityDelta;
      }

      entity->SetVelocityY( min( newVelocityY, entity->GetMaxGravityVelocity() ) );
   }
}

void EntityPhysics::ApplyFriction( const shared_ptr<Entity> entity )
{
   if ( ( entity->GetEntityType() == EntityType::Player && _frameActionRegistry->ActionFlagged( FrameAction::PlayerPushed ) ) ||
        entity->IsKnockedBack() )
   {
      return;
   }

   auto deceleration = entity->GetFrictionDecelerationPerSecond();
   if ( deceleration == 0 )
   {
      return;
   }

   auto velocityDelta = deceleration * _frameRateProvider->GetFrameSeconds();
   auto currentVelocityX = entity->GetVelocityX();
   float newVelocityX = 0;

   if ( currentVelocityX < 0 )
   {
      newVelocityX = min( currentVelocityX + velocityDelta, 0.0f );
   }
   else if ( currentVelocityX > 0 )
   {
      newVelocityX = max( currentVelocityX - velocityDelta, 0.0f );
   }

   entity->SetVelocityX( newVelocityX );
}
