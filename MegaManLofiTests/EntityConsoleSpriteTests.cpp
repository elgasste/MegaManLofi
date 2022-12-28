#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/EntityConsoleSprite.h>

#include "mock_ConsoleSprite.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class EntityConsoleSpriteTests : public Test
{
public:
   void SetUp() override
   {
      _entitySprite.reset( new EntityConsoleSprite );
   }

protected:
   shared_ptr<EntityConsoleSprite> _entitySprite;
};

TEST_F( EntityConsoleSpriteTests, Tick_Always_TicksCurrentSprite )
{
   auto spriteMock1 = shared_ptr<mock_ConsoleSprite>( new NiceMock<mock_ConsoleSprite> );
   auto spriteMock2 = shared_ptr<mock_ConsoleSprite>( new NiceMock<mock_ConsoleSprite> );
   _entitySprite->AddSprite( MovementType::Standing, Direction::Left, spriteMock1 );
   _entitySprite->AddSprite( MovementType::Walking, Direction::Right, spriteMock2 );

   _entitySprite->SetMovementType( MovementType::Standing );
   _entitySprite->SetDirection( Direction::Left );

   EXPECT_CALL( *spriteMock1, Tick() );

   _entitySprite->Tick();
}

TEST_F( EntityConsoleSpriteTests, Reset_Always_ResetsAllExistingSprites )
{
   auto spriteMock1 = shared_ptr<mock_ConsoleSprite>( new NiceMock<mock_ConsoleSprite> );
   auto spriteMock2 = shared_ptr<mock_ConsoleSprite>( new NiceMock<mock_ConsoleSprite> );
   auto spriteMock3 = shared_ptr<mock_ConsoleSprite>( new NiceMock<mock_ConsoleSprite> );
   _entitySprite->AddSprite( MovementType::Standing, Direction::Left, spriteMock1 );
   _entitySprite->AddSprite( MovementType::Walking, Direction::Right, spriteMock2 );
   _entitySprite->AddSprite( MovementType::Standing, Direction::Up, spriteMock3 );

   EXPECT_CALL( *spriteMock1, Reset() );
   EXPECT_CALL( *spriteMock2, Reset() );
   EXPECT_CALL( *spriteMock3, Reset() );

   _entitySprite->Reset();
}

TEST_F( EntityConsoleSpriteTests, GetWidth_Always_ReturnsCurrentSpriteWidth )
{
   auto spriteMock1 = shared_ptr<mock_ConsoleSprite>( new NiceMock<mock_ConsoleSprite> );
   auto spriteMock2 = shared_ptr<mock_ConsoleSprite>( new NiceMock<mock_ConsoleSprite> );
   _entitySprite->AddSprite( MovementType::Standing, Direction::Left, spriteMock1 );
   _entitySprite->AddSprite( MovementType::Walking, Direction::Right, spriteMock2 );

   EXPECT_CALL( *spriteMock2, GetWidth() ).WillOnce( Return( 3 ) );

   _entitySprite->SetMovementType( MovementType::Walking );
   _entitySprite->SetDirection( Direction::Right );

   EXPECT_EQ( _entitySprite->GetWidth(), 3 );
}

TEST_F( EntityConsoleSpriteTests, GetHeight_Always_ReturnsCurrentSpriteHeight )
{
   auto spriteMock1 = shared_ptr<mock_ConsoleSprite>( new NiceMock<mock_ConsoleSprite> );
   auto spriteMock2 = shared_ptr<mock_ConsoleSprite>( new NiceMock<mock_ConsoleSprite> );
   _entitySprite->AddSprite( MovementType::Standing, Direction::Left, spriteMock1 );
   _entitySprite->AddSprite( MovementType::Walking, Direction::Right, spriteMock2 );

   EXPECT_CALL( *spriteMock1, GetHeight() ).WillOnce( Return( 7 ) );

   _entitySprite->SetMovementType( MovementType::Standing );
   _entitySprite->SetDirection( Direction::Left );

   EXPECT_EQ( _entitySprite->GetHeight(), 7 );
}

TEST_F( EntityConsoleSpriteTests, GetTotalTraversalSeconds_Always_ReturnsCurrentSpriteTotalTraversalSeconds )
{
   auto spriteMock1 = shared_ptr<mock_ConsoleSprite>( new NiceMock<mock_ConsoleSprite> );
   auto spriteMock2 = shared_ptr<mock_ConsoleSprite>( new NiceMock<mock_ConsoleSprite> );
   _entitySprite->AddSprite( MovementType::Standing, Direction::Left, spriteMock1 );
   _entitySprite->AddSprite( MovementType::Walking, Direction::Right, spriteMock2 );

   EXPECT_CALL( *spriteMock2, GetTotalTraversalSeconds() ).WillOnce( Return( 8.2 ) );

   _entitySprite->SetMovementType( MovementType::Walking );
   _entitySprite->SetDirection( Direction::Right );

   EXPECT_EQ( _entitySprite->GetTotalTraversalSeconds(), 8.2 );
}

TEST_F( EntityConsoleSpriteTests, GetCurrentImage_Always_ReturnsCurrentSpriteCurrentImage )
{
   auto spriteMock1 = shared_ptr<mock_ConsoleSprite>( new NiceMock<mock_ConsoleSprite> );
   auto spriteMock2 = shared_ptr<mock_ConsoleSprite>( new NiceMock<mock_ConsoleSprite> );
   _entitySprite->AddSprite( MovementType::Standing, Direction::Left, spriteMock1 );
   _entitySprite->AddSprite( MovementType::Walking, Direction::Right, spriteMock2 );
   ConsoleImage image;
   image.Width = 1000;

   EXPECT_CALL( *spriteMock1, GetCurrentImage() ).WillOnce( ReturnRef( image ) );

   _entitySprite->SetMovementType( MovementType::Standing );
   _entitySprite->SetDirection( Direction::Left );

   EXPECT_EQ( _entitySprite->GetCurrentImage().Width, 1000 );
}
