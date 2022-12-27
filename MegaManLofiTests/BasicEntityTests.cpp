#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/BasicEntity.h>

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class BasicEntityTests : public Test { };

TEST_F( BasicEntityTests, Constructor_Always_InitializesProperties )
{
   auto entity = make_shared<BasicEntity>();

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
}

TEST_F( BasicEntityTests, Setters_Always_SetsPropertyValues )
{
   auto entity = make_shared<BasicEntity>();

   entity->SetUniqueId( 12 );
   entity->SetEntityType( EntityType::Projectile );
   entity->SetEntityMetaId( 5 );
   entity->SetArenaPosition( { 1, 2 } );
   entity->SetVelocityX( 3 );
   entity->SetVelocityY( 4 );
   entity->SetDirection( Direction::Right );
   entity->SetHitBox( { 6, 7, 8, 9 } );

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
}
