#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/EntityFactory.h>
#include <MegaManLofi/EntityDefs.h>
#include <MegaManLofi/Entity.h>

#include "mock_UniqueNumberGenerator.h"
#include "mock_FrameRateProvider.h"
#include "mock_PlayerInfoProvider.h"
#include "mock_GameCommandExecutor.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class EntityFactoryTests : public Test
{
public:
   void SetUp() override
   {
      _entityDefs.reset( new EntityDefs );
      _uniqueNumberGeneratorMock.reset( new NiceMock<mock_UniqueNumberGenerator> );
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );
      _playerInfoProviderMock.reset( new NiceMock<mock_PlayerInfoProvider> );
      _commandExecutorMock.reset( new NiceMock<mock_GameCommandExecutor> );

      _entityDefs->EntityTypeMap[1] = EntityType::Item;
      _entityDefs->EntityTypeMap[2] = EntityType::Projectile;

      _entityDefs->ItemInfoMap[1].HitBox = { 1, 1, 20, 20 };
      _entityDefs->ItemInfoMap[1].MaxGravityVelocity = 100;
      _entityDefs->ItemInfoMap[1].GravityAccelerationPerSecond = 3'000;

      _entityDefs->ProjectileInfoMap[2].HitBox = { 2, 2, 10, 10 };
      _entityDefs->ProjectileInfoMap[2].Velocity = 100;

      ON_CALL( *_uniqueNumberGeneratorMock, GetNext() ).WillByDefault( Return( 3 ) );
   }

   void BuildFactory()
   {
      _factory.reset( new EntityFactory( _entityDefs, _uniqueNumberGeneratorMock, _frameRateProviderMock ) );
      _factory->Initialize( _playerInfoProviderMock, _commandExecutorMock );
   }

protected:
   shared_ptr<EntityDefs> _entityDefs;
   shared_ptr<mock_UniqueNumberGenerator> _uniqueNumberGeneratorMock;
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<mock_PlayerInfoProvider> _playerInfoProviderMock;
   shared_ptr<mock_GameCommandExecutor> _commandExecutorMock;

   shared_ptr<EntityFactory> _factory;
};

TEST_F( EntityFactoryTests, CreateEntity_Always_GetsUniqueIdFromNumberGenerator )
{
   BuildFactory();

   EXPECT_CALL( *_uniqueNumberGeneratorMock, GetNext() ).WillOnce( Return( 33 ) );

   auto entity = _factory->CreateEntity( 1, { 0, 0 }, Direction::Left );

   EXPECT_EQ( entity->GetUniqueId(), 33 );
}

TEST_F( EntityFactoryTests, CreateEntity_Always_SetsEntityTypeFromDefs )
{
   BuildFactory();

   auto entity1 = _factory->CreateEntity( 1, { 0, 0 }, Direction::Left );
   auto entity2 = _factory->CreateEntity( 2, { 0, 0 }, Direction::Left );

   EXPECT_EQ( entity1->GetEntityType(), EntityType::Item );
   EXPECT_EQ( entity2->GetEntityType(), EntityType::Projectile );
}

TEST_F( EntityFactoryTests, CreateEntity_Always_SetsEntityMetaId )
{
   BuildFactory();

   auto entity = _factory->CreateEntity( 2, { 0, 0 }, Direction::Left );

   EXPECT_EQ( entity->GetEntityMetaId(), 2 );
}

TEST_F( EntityFactoryTests, CreateEntity_Always_SetsArenaPosition )
{
   BuildFactory();

   auto entity = _factory->CreateEntity( 2, { 2, 3 }, Direction::UpRight );

   EXPECT_EQ( entity->GetArenaPositionLeft(), 2 );
   EXPECT_EQ( entity->GetArenaPositionTop(), 3 );
}

TEST_F( EntityFactoryTests, CreateEntity_NonProjectileAlways_SetsDirection )
{
   BuildFactory();

   auto entity = _factory->CreateEntity( 1, { 0, 0 }, Direction::UpRight );

   EXPECT_EQ( entity->GetDirection(), Direction::UpRight );
}

TEST_F( EntityFactoryTests, CreateEntity_Item_SetsItemPropertiesFromDefs )
{
   BuildFactory();

   auto entity = _factory->CreateEntity( 1, { 0, 0 }, Direction::Left );

   EXPECT_EQ( entity->GetHitBox().Left, 1 );
   EXPECT_EQ( entity->GetHitBox().Top, 1 );
   EXPECT_EQ( entity->GetHitBox().Width, 20 );
   EXPECT_EQ( entity->GetHitBox().Height, 20 );
   EXPECT_EQ( entity->GetMaxGravityVelocity(), 100.0f );
   EXPECT_EQ( entity->GetGravityAccelerationPerSecond(), 3'000.0f );
   EXPECT_EQ( entity->GetMaxHealth(), 1 );
   EXPECT_EQ( entity->GetHealth(), 1 );
}

TEST_F( EntityFactoryTests, CreateEntity_Projectile_SetsHealthPropertiesFromDefs )
{
   BuildFactory();

   auto entity = _factory->CreateEntity( 2, { 0, 0 }, Direction::Left );

   EXPECT_EQ( entity->GetMaxHealth(), 1 );
   EXPECT_EQ( entity->GetHealth(), 1 );
}

TEST_F( EntityFactoryTests, CreateEntity_ProjectileMovingLeft_SetsVelocityFromDefs )
{
   BuildFactory();

   auto entity = _factory->CreateEntity( 2, { 0, 0 }, Direction::Left );

   EXPECT_EQ( entity->GetVelocityX(), -100 );
   EXPECT_EQ( entity->GetVelocityY(), 0 );
}

TEST_F( EntityFactoryTests, CreateEntity_ProjectileMovingUpLeft_SetsVelocityFromDefs )
{
   BuildFactory();

   auto entity = _factory->CreateEntity( 2, { 0, 0 }, Direction::UpLeft );

   auto angle = (float)atan2( -1.0f, -1.0f );
   auto velocityX = (float)cos( angle ) * 100.0f;
   auto velocityY = (float)sin( angle ) * 100.0f;
   EXPECT_EQ( entity->GetVelocityX(), velocityX );
   EXPECT_EQ( entity->GetVelocityY(), velocityY );
}

TEST_F( EntityFactoryTests, CreateEntity_ProjectileMovingUp_SetsVelocityFromDefs )
{
   BuildFactory();

   auto entity = _factory->CreateEntity( 2, { 0, 0 }, Direction::Up );

   EXPECT_EQ( entity->GetVelocityX(), 0 );
   EXPECT_EQ( entity->GetVelocityY(), -100 );
}

TEST_F( EntityFactoryTests, CreateEntity_ProjectileMovingUpRight_SetsVelocityFromDefs )
{
   BuildFactory();

   auto entity = _factory->CreateEntity( 2, { 0, 0 }, Direction::UpRight );

   auto angle = (float)atan2( -1.0f, 1.0f );
   auto velocityX = (float)cos( angle ) * 100.0f;
   auto velocityY = (float)sin( angle ) * 100.0f;
   EXPECT_EQ( entity->GetVelocityX(), velocityX );
   EXPECT_EQ( entity->GetVelocityY(), velocityY );
}

TEST_F( EntityFactoryTests, CreateEntity_ProjectileMovingRight_SetsVelocityFromDefs )
{
   BuildFactory();

   auto entity = _factory->CreateEntity( 2, { 0, 0 }, Direction::Right );

   EXPECT_EQ( entity->GetVelocityX(), 100 );
   EXPECT_EQ( entity->GetVelocityY(), 0 );
}

TEST_F( EntityFactoryTests, CreateEntity_ProjectileMovingDownRight_SetsVelocityFromDefs )
{
   BuildFactory();

   auto entity = _factory->CreateEntity( 2, { 0, 0 }, Direction::DownRight );

   auto angle = (float)atan2( 1.0f, 1.0f );
   auto velocityX = (float)cos( angle ) * 100.0f;
   auto velocityY = (float)sin( angle ) * 100.0f;
   EXPECT_EQ( entity->GetVelocityX(), velocityX );
   EXPECT_EQ( entity->GetVelocityY(), velocityY );
}

TEST_F( EntityFactoryTests, CreateEntity_ProjectileMovingDown_SetsVelocityFromDefs )
{
   BuildFactory();

   auto entity = _factory->CreateEntity( 2, { 0, 0 }, Direction::Down );

   EXPECT_EQ( entity->GetVelocityX(), 0 );
   EXPECT_EQ( entity->GetVelocityY(), 100 );
}

TEST_F( EntityFactoryTests, CreateEntity_ProjectileMovingDownLeft_SetsVelocityFromDefs )
{
   BuildFactory();

   auto entity = _factory->CreateEntity( 2, { 0, 0 }, Direction::DownLeft );

   auto angle = (float)atan2( 1.0f, -1.0f );
   auto velocityX = (float)cos( angle ) * 100.0f;
   auto velocityY = (float)sin( angle ) * 100.0f;
   EXPECT_EQ( entity->GetVelocityX(), velocityX );
   EXPECT_EQ( entity->GetVelocityY(), velocityY );
}

TEST_F( EntityFactoryTests, CreateProjectile_WithTargetInUpperLeft_SetsCorrectVelocity )
{
   BuildFactory();

   auto projectile = _factory->CreateProjectile( 2, { 0, 0 }, { -6.0f, -5.0f } );

   auto angle = (float)atan2( -5.0f, -6.0f );
   auto velocityX = (float)cos( angle ) * 100.0f;
   auto velocityY = (float)sin( angle ) * 100.0f;
   EXPECT_EQ( projectile->GetVelocityX(), velocityX );
   EXPECT_EQ( projectile->GetVelocityY(), velocityY );
}

TEST_F( EntityFactoryTests, CreateProjectile_WithTargetInUpperRight_SetsCorrectVelocity )
{
   BuildFactory();

   auto projectile = _factory->CreateProjectile( 2, { 0, 0 }, { 6.0f, -5.0f } );

   auto angle = (float)atan2( -5.0f, 6.0f );
   auto velocityX = (float)cos( angle ) * 100.0f;
   auto velocityY = (float)sin( angle ) * 100.0f;
   EXPECT_EQ( projectile->GetVelocityX(), velocityX );
   EXPECT_EQ( projectile->GetVelocityY(), velocityY );
}

TEST_F( EntityFactoryTests, CreateProjectile_WithTargetInLowerRight_SetsCorrectVelocity )
{
   BuildFactory();

   auto projectile = _factory->CreateProjectile( 2, { 0, 0 }, { 6.0f, 5.0f } );

   auto angle = (float)atan2( 5.0f, 6.0f );
   auto velocityX = (float)cos( angle ) * 100.0f;
   auto velocityY = (float)sin( angle ) * 100.0f;
   EXPECT_EQ( projectile->GetVelocityX(), velocityX );
   EXPECT_EQ( projectile->GetVelocityY(), velocityY );
}

TEST_F( EntityFactoryTests, CreateProjectile_WithTargetInLowerLeft_SetsCorrectVelocity )
{
   BuildFactory();

   auto projectile = _factory->CreateProjectile( 2, { 0, 0 }, { -6.0f, 5.0f } );

   auto angle = (float)atan2( 5.0f, -6.0f );
   auto velocityX = (float)cos( angle ) * 100.0f;
   auto velocityY = (float)sin( angle ) * 100.0f;
   EXPECT_EQ( projectile->GetVelocityX(), velocityX );
   EXPECT_EQ( projectile->GetVelocityY(), velocityY );
}
