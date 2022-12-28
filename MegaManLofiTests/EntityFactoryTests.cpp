#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/EntityFactory.h>
#include <MegaManLofi/EntityDefs.h>
#include <MegaManLofi/IEntity.h>

#include "mock_UniqueNumberGenerator.h"

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

      _entityDefs->BulletEntityMetaId = 4;
      _entityDefs->BulletVelocity = 10;
      _entityDefs->BulletHitBox = { 1, 2, 3, 4 };

      ON_CALL( *_uniqueNumberGeneratorMock, GetNext() ).WillByDefault( Return( 3 ) );
   }

   void BuildFactory()
   {
      _factory.reset( new EntityFactory( _entityDefs, _uniqueNumberGeneratorMock ) );
   }

protected:
   shared_ptr<EntityDefs> _entityDefs;
   shared_ptr<mock_UniqueNumberGenerator> _uniqueNumberGeneratorMock;

   shared_ptr<EntityFactory> _factory;
};

TEST_F( EntityFactoryTests, CreateBullet_Always_SetsPropertiesBasedOnDefs )
{
   BuildFactory();

   auto bullet = _factory->CreateBullet( { 7, 8 }, Direction::Right );

   EXPECT_EQ( bullet->GetEntityType(), EntityType::Projectile );
   EXPECT_EQ( bullet->GetEntityMetaId(), 4 );
   EXPECT_EQ( bullet->GetArenaPosition().Left, 7 );
   EXPECT_EQ( bullet->GetArenaPosition().Top, 8 );
   EXPECT_EQ( bullet->GetDirection(), Direction::Right );
   EXPECT_EQ( bullet->GetHitBox().Left, 1 );
   EXPECT_EQ( bullet->GetHitBox().Top, 2 );
   EXPECT_EQ( bullet->GetHitBox().Width, 3 );
   EXPECT_EQ( bullet->GetHitBox().Height, 4 );
}

TEST_F( EntityFactoryTests, CreateBullet_Always_GetsUniqueIdFromNumberGenerator )
{
   BuildFactory();

   EXPECT_CALL( *_uniqueNumberGeneratorMock, GetNext() ).WillOnce( Return( 33 ) );
   auto bullet = _factory->CreateBullet( { 7, 8 }, Direction::Right );

   EXPECT_EQ( bullet->GetUniqueId(), 33 );
}

TEST_F( EntityFactoryTests, CreateBullet_MovingLeft_SetsCorrectVelocityValues )
{
   BuildFactory();

   auto bullet = _factory->CreateBullet( { 0, 0 }, Direction::Left );

   EXPECT_EQ( bullet->GetVelocityX(), -10 );
   EXPECT_EQ( bullet->GetVelocityY(), 0 );
}

TEST_F( EntityFactoryTests, CreateBullet_MovingUpLeft_SetsCorrectVelocityValues )
{
   BuildFactory();

   auto bullet = _factory->CreateBullet( { 0, 0 }, Direction::UpLeft );

   EXPECT_EQ( bullet->GetVelocityX(), -10 );
   EXPECT_EQ( bullet->GetVelocityY(), -10 );
}

TEST_F( EntityFactoryTests, CreateBullet_MovingUp_SetsCorrectVelocityValues )
{
   BuildFactory();

   auto bullet = _factory->CreateBullet( { 0, 0 }, Direction::Up );

   EXPECT_EQ( bullet->GetVelocityX(), 0 );
   EXPECT_EQ( bullet->GetVelocityY(), -10 );
}

TEST_F( EntityFactoryTests, CreateBullet_MovingUpRight_SetsCorrectVelocityValues )
{
   BuildFactory();

   auto bullet = _factory->CreateBullet( { 0, 0 }, Direction::UpRight );

   EXPECT_EQ( bullet->GetVelocityX(), 10 );
   EXPECT_EQ( bullet->GetVelocityY(), -10 );
}

TEST_F( EntityFactoryTests, CreateBullet_MovingRight_SetsCorrectVelocityValues )
{
   BuildFactory();

   auto bullet = _factory->CreateBullet( { 0, 0 }, Direction::Right );

   EXPECT_EQ( bullet->GetVelocityX(), 10 );
   EXPECT_EQ( bullet->GetVelocityY(), 0 );
}

TEST_F( EntityFactoryTests, CreateBullet_MovingDownRight_SetsCorrectVelocityValues )
{
   BuildFactory();

   auto bullet = _factory->CreateBullet( { 0, 0 }, Direction::DownRight );

   EXPECT_EQ( bullet->GetVelocityX(), 10 );
   EXPECT_EQ( bullet->GetVelocityY(), 10 );
}

TEST_F( EntityFactoryTests, CreateBullet_MovingDown_SetsCorrectVelocityValues )
{
   BuildFactory();

   auto bullet = _factory->CreateBullet( { 0, 0 }, Direction::Down );

   EXPECT_EQ( bullet->GetVelocityX(), 0 );
   EXPECT_EQ( bullet->GetVelocityY(), 10 );
}

TEST_F( EntityFactoryTests, CreateBullet_MovingDownLeft_SetsCorrectVelocityValues )
{
   BuildFactory();

   auto bullet = _factory->CreateBullet( { 0, 0 }, Direction::DownLeft );

   EXPECT_EQ( bullet->GetVelocityX(), -10 );
   EXPECT_EQ( bullet->GetVelocityY(), 10 );
}
