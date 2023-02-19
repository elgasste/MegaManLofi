#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/Entity.h>

#include "mock_Behavior.h"
#include "mock_FrameRateProvider.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class EntityTests : public Test { };

TEST_F( EntityTests, Constructor_Always_InitializesProperties )
{
   auto entity = make_shared<Entity>();

   EXPECT_EQ( entity->GetUniqueId(), 0 );
   EXPECT_EQ( entity->GetEntityType(), (EntityType)0 );
   EXPECT_EQ( entity->GetEntityMetaId(), 0 );
   EXPECT_EQ( entity->GetArenaPosition().Left, 0 );
   EXPECT_EQ( entity->GetArenaPosition().Top, 0 );
   EXPECT_EQ( entity->GetVelocityX(), 0 );
   EXPECT_EQ( entity->GetVelocityY(), 0 );
   EXPECT_EQ( entity->GetDirection(), (Direction)0 );
   EXPECT_EQ( entity->GetHitBox().Left, 0 );
   EXPECT_EQ( entity->GetHitBox().Top, 0 );
   EXPECT_EQ( entity->GetHitBox().Width, 0 );
   EXPECT_EQ( entity->GetHitBox().Height, 0 );
   EXPECT_EQ( entity->GetMovementType(), (MovementType)0 );
}

TEST_F( EntityTests, Setters_Always_SetsPropertyValues )
{
   auto entity = make_shared<Entity>();

   entity->SetUniqueId( 12 );
   entity->SetEntityType( EntityType::Projectile );
   entity->SetEntityMetaId( 5 );
   entity->SetArenaPosition( { 1, 2 } );
   entity->SetVelocityX( 3 );
   entity->SetVelocityY( 4 );
   entity->SetDirection( Direction::Right );
   entity->SetHitBox( { 6, 7, 8, 9 } );
   entity->SetMovementType( MovementType::Airborne );

   EXPECT_EQ( entity->GetUniqueId(), 12 );
   EXPECT_EQ( entity->GetEntityType(), EntityType::Projectile );
   EXPECT_EQ( entity->GetEntityMetaId(), 5 );
   EXPECT_EQ( entity->GetArenaPosition().Left, 1 );
   EXPECT_EQ( entity->GetArenaPosition().Top, 2 );
   EXPECT_EQ( entity->GetVelocityX(), 3 );
   EXPECT_EQ( entity->GetVelocityY(), 4 );
   EXPECT_EQ( entity->GetDirection(), Direction::Right );
   EXPECT_EQ( entity->GetHitBox().Left, 6 );
   EXPECT_EQ( entity->GetHitBox().Top, 7 );
   EXPECT_EQ( entity->GetHitBox().Width, 8 );
   EXPECT_EQ( entity->GetHitBox().Height, 9 );
   EXPECT_EQ( entity->GetMovementType(), MovementType::Airborne );
}

TEST_F( EntityTests, SetVelocityX_EntityIsKnockedBack_DoesNotChangeVelocityX )
{
   auto entity = make_shared<Entity>();
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );
   entity->SetKnockBackSeconds( 1 );

   EXPECT_TRUE( entity->TakeCollisionPayload( { -10, 0 }, 0 ) );
   EXPECT_EQ( entity->GetVelocityX(), 0 );

   entity->SetVelocityX( 100 );

   EXPECT_EQ( entity->GetVelocityX(), 0 );
}

TEST_F( EntityTests, SetVelocityX_EntityIsKnockedBack_DoesNotChangeVelocityY )
{
   auto entity = make_shared<Entity>();
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );
   entity->SetKnockBackSeconds( 1 );

   EXPECT_TRUE( entity->TakeCollisionPayload( { -10, 0 }, 0 ) );
   EXPECT_EQ( entity->GetVelocityY(), 0 );

   entity->SetVelocityY( 100 );

   EXPECT_EQ( entity->GetVelocityY(), 0 );
}

TEST_F( EntityTests, Tick_IsInvulnerableWithTimeRemaining_RemainsInvulnerable )
{
   auto frameRateProviderMock = shared_ptr<mock_FrameRateProvider>( new NiceMock<mock_FrameRateProvider> );
   ON_CALL( *frameRateProviderMock, GetFrameSeconds() ).WillByDefault( Return( 1.0f ) );
   auto entity = shared_ptr<Entity>( new Entity( frameRateProviderMock ) );
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );
   entity->SetDamageInvulnerabilitySeconds( 1.5f );

   EXPECT_TRUE( entity->TakeCollisionPayload( { -10, 0 }, 0 ) );
   entity->Tick();

   EXPECT_TRUE( entity->IsInvulnerable() );
}

TEST_F( EntityTests, Tick_IsInvulnerableWithNoTimeRemaining_BecomesVulnerable )
{
   auto frameRateProviderMock = shared_ptr<mock_FrameRateProvider>( new NiceMock<mock_FrameRateProvider> );
   ON_CALL( *frameRateProviderMock, GetFrameSeconds() ).WillByDefault( Return( 1.0f ) );
   auto entity = shared_ptr<Entity>( new Entity( frameRateProviderMock ) );
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );
   entity->SetDamageInvulnerabilitySeconds( 0.9f );

   EXPECT_TRUE( entity->TakeCollisionPayload( { -10, 0 }, 0 ) );
   entity->Tick();

   EXPECT_FALSE( entity->IsInvulnerable() );
}

TEST_F( EntityTests, Tick_IsKnockedBackWithTimeRemaining_RemainsKnockedBack )
{
   auto frameRateProviderMock = shared_ptr<mock_FrameRateProvider>( new NiceMock<mock_FrameRateProvider> );
   ON_CALL( *frameRateProviderMock, GetFrameSeconds() ).WillByDefault( Return( 1.0f ) );
   auto entity = shared_ptr<Entity>( new Entity( frameRateProviderMock ) );
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );
   entity->SetKnockBackSeconds( 1.5f );

   EXPECT_TRUE( entity->TakeCollisionPayload( { -10, 0 }, 0 ) );
   entity->Tick();

   EXPECT_TRUE( entity->IsKnockedBack() );
}

TEST_F( EntityTests, Tick_IsKnockedBackWithNoTimeRemaining_BecomesNotKnockedBack )
{
   auto frameRateProviderMock = shared_ptr<mock_FrameRateProvider>( new NiceMock<mock_FrameRateProvider> );
   ON_CALL( *frameRateProviderMock, GetFrameSeconds() ).WillByDefault( Return( 1.0f ) );
   auto entity = shared_ptr<Entity>( new Entity( frameRateProviderMock ) );
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );
   entity->SetKnockBackSeconds( 0.9f );

   EXPECT_TRUE( entity->TakeCollisionPayload( { -10, 0 }, 0 ) );
   entity->Tick();

   EXPECT_FALSE( entity->IsKnockedBack() );
}

TEST_F( EntityTests, Tick_BehaviorIsNotNull_TicksBehavior )
{
   auto entity = make_shared<Entity>();
   auto behaviorMock = shared_ptr<mock_Behavior>( new NiceMock<mock_Behavior> );
   entity->SetBehavior( behaviorMock );

   EXPECT_CALL( *behaviorMock, Tick() );

   entity->Tick();
}

TEST_F( EntityTests, TakeCollisionPayload_HealthPayloadIsZero_ReturnsFalse )
{
   auto entity = make_shared<Entity>();

   EXPECT_FALSE( entity->TakeCollisionPayload( EntityCollisionPayload(), 0 ) );
}

TEST_F( EntityTests, TakeCollisionPayload_HealthPayloadIsNegativeAndIsInvulnerable_ReturnsFalse )
{
   auto entity = make_shared<Entity>();
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );
   entity->SetDamageInvulnerabilitySeconds( 1.0f );

   EXPECT_TRUE( entity->TakeCollisionPayload( { -10, 0 }, 0 ) );

   EXPECT_EQ( entity->GetHealth(), 90 );
   EXPECT_TRUE( entity->IsInvulnerable() );

   EXPECT_FALSE( entity->TakeCollisionPayload( { -10, 0 }, 0 ) );
   EXPECT_EQ( entity->GetHealth(), 90 );
}

TEST_F( EntityTests, TakeCollisionPayload_NewHealthIsTooLow_ClampsHealthToZero )
{
   auto entity = make_shared<Entity>();
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );

   EXPECT_TRUE( entity->TakeCollisionPayload( { -150, 0 }, 0 ) );
   EXPECT_EQ( entity->GetHealth(), 0 );
}

TEST_F( EntityTests, TakeCollisionPayload_NewHealthMatchesOldHealth_ReturnsFalse )
{
   auto entity = make_shared<Entity>();
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );

   EXPECT_FALSE( entity->TakeCollisionPayload( { 10, 0 }, 0 ) );
}

TEST_F( EntityTests, TakeCollisionPayload_NewHealthIsTooHigh_ClampsHealthToMax )
{
   auto entity = make_shared<Entity>();
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 80 );

   EXPECT_TRUE( entity->TakeCollisionPayload( { 30, 0 }, 0 ) );
   EXPECT_EQ( entity->GetHealth(), 100 );
}

TEST_F( EntityTests, TakeCollisionPayload_HealthDecreasedToZero_DoesNotSetInvulnerability )
{
   auto entity = make_shared<Entity>();
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );
   entity->SetDamageInvulnerabilitySeconds( 1.0f );

   EXPECT_TRUE( entity->TakeCollisionPayload( { -100, 0 }, 0 ) );

   EXPECT_EQ( entity->GetHealth(), 0 );
   EXPECT_FALSE( entity->IsInvulnerable() );
}

TEST_F( EntityTests, TakeCollisionPayload_HealthDecreasedAndNoInvulnerability_DoesNotSetInvulnerability )
{
   auto entity = make_shared<Entity>();
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );

   EXPECT_TRUE( entity->TakeCollisionPayload( { -10, 0 }, 0 ) );

   EXPECT_EQ( entity->GetHealth(), 90 );
   EXPECT_FALSE( entity->IsInvulnerable() );
}

TEST_F( EntityTests, TakeCollisionPayload_HealthDecreasedAndHasInvulnerability_SetsInvulnerability )
{
   auto entity = make_shared<Entity>();
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );
   entity->SetDamageInvulnerabilitySeconds( 1.0f );

   EXPECT_TRUE( entity->TakeCollisionPayload( { -10, 0 }, 0 ) );

   EXPECT_EQ( entity->GetHealth(), 90 );
   EXPECT_TRUE( entity->IsInvulnerable() );
}

TEST_F( EntityTests, TakeCollisionPayload_HealthDecreasedAndNoKnockBack_DoesNotSetKnockBack )
{
   auto entity = make_shared<Entity>();
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );
   entity->SetKnockBackSeconds( 0 );

   EXPECT_TRUE( entity->TakeCollisionPayload( { -10, 0 }, 0 ) );

   EXPECT_FALSE( entity->IsKnockedBack() );
}

TEST_F( EntityTests, TakeCollisionPayload_HealthDecreasedAndHasKnockBack_StopsYVelocity )
{
   auto entity = make_shared<Entity>();
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );
   entity->SetKnockBackSeconds( 1 );

   entity->SetVelocityY( -50 );
   EXPECT_EQ( entity->GetVelocityY(), -50 );

   EXPECT_TRUE( entity->TakeCollisionPayload( { -10, 0 }, -1 ) );

   EXPECT_EQ( entity->GetVelocityY(), 0 );
}

TEST_F( EntityTests, TakeCollisionPayload_HealthDecreasedAndGiverIsMovingLeft_KnocksBackToTheLeft )
{
   auto entity = make_shared<Entity>();
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );
   entity->SetKnockBackSeconds( 0.5f );
   entity->SetKnockBackVelocity( 400 );

   EXPECT_TRUE( entity->TakeCollisionPayload( { -10, 0 }, -1 ) );

   EXPECT_TRUE( entity->IsKnockedBack() );
   EXPECT_EQ( entity->GetVelocityX(), -400 );
}

TEST_F( EntityTests, TakeCollisionPayload_HealthDecreasedAndGiverIsMovingRight_KnocksBackToTheRight )
{
   auto entity = make_shared<Entity>();
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );
   entity->SetKnockBackSeconds( 0.5f );
   entity->SetKnockBackVelocity( 400 );

   EXPECT_TRUE( entity->TakeCollisionPayload( { -10, 0 }, 1 ) );

   EXPECT_TRUE( entity->IsKnockedBack() );
   EXPECT_EQ( entity->GetVelocityX(), 400 );
}

TEST_F( EntityTests, TakeCollisionPayload_HealthDecreasedAndGiverIsNotMovingAndEntityIsFacingLeftward_KnocksBackToTheRight )
{
   auto entity = make_shared<Entity>();
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );
   entity->SetKnockBackSeconds( 0.5f );
   entity->SetKnockBackVelocity( 400 );
   entity->SetDirection( Direction::DownLeft );

   EXPECT_TRUE( entity->TakeCollisionPayload( { -10, 0 }, 0 ) );

   EXPECT_TRUE( entity->IsKnockedBack() );
   EXPECT_EQ( entity->GetVelocityX(), 400 );
}

TEST_F( EntityTests, TakeCollisionPayload_HealthDecreasedAndGiverIsNotMovingAndEntityIsNotFacingLeftward_KnocksBackToTheLeft )
{
   auto entity = make_shared<Entity>();
   entity->SetMaxHealth( 100 );
   entity->SetHealth( 100 );
   entity->SetKnockBackSeconds( 0.5f );
   entity->SetKnockBackVelocity( 400 );
   entity->SetDirection( Direction::Right );

   EXPECT_TRUE( entity->TakeCollisionPayload( { -10, 0 }, 0 ) );

   EXPECT_TRUE( entity->IsKnockedBack() );
   EXPECT_EQ( entity->GetVelocityX(), -400 );
}
