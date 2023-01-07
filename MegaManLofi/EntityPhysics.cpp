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

   if ( entity->GetEntityType() == EntityType::Player )
   {
      if ( currentVelocityY < 0 )
      {
         auto newVelocityY = _frameActionRegistry->ActionFlagged( FrameAction::PlayerJumping ) ? currentVelocityY + gravityVelocityDelta : 0;
         entity->SetVelocityY( newVelocityY );
      }
      else
      {
         entity->SetVelocityY( min( currentVelocityY + gravityVelocityDelta, entity->GetMaxGravityVelocity() ) );
      }
   }
   else if ( entity->GetEntityType() == EntityType::Item )
   {
      entity->SetVelocityY( min( currentVelocityY + gravityVelocityDelta, entity->GetMaxGravityVelocity() ) );
   }
}

// MUFFINS: test this
void EntityPhysics::ApplyFriction( const shared_ptr<Entity> entity )
{
   if ( entity->GetEntityType() == EntityType::Player && _frameActionRegistry->ActionFlagged( FrameAction::PlayerPushed ) )
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
