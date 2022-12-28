#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/Arena.h>
#include <MegaManLofi/ArenaDefs.h>
#include <MegaManLofi/FrameAction.h>

#include "mock_GameEventAggregator.h"
#include "mock_Player.h"
#include "mock_FrameActionRegistry.h"
#include "mock_FrameRateProvider.h"
#include "mock_Entity.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class ArenaTests : public Test
{
public:
   void SetUp() override
   {
      _arenaDefs.reset( new ArenaDefs );
      _eventAggregatorMock.reset( new NiceMock<mock_GameEventAggregator> );
      _playerMock.reset( new NiceMock<mock_Player> );

      _arenaDefs->DefaultTileWidth = 2;
      _arenaDefs->DefaultTileHeight = 2;
      _arenaDefs->DefaultHorizontalTiles = 10;
      _arenaDefs->DefaultVerticalTiles = 8;
      _arenaDefs->DefaultPlayerPosition = { 10, 8 };

      for ( int i = 0; i < _arenaDefs->DefaultHorizontalTiles * _arenaDefs->DefaultVerticalTiles; i++ )
      {
         _arenaDefs->DefaultTiles.push_back( { true, true, true, true } );
      }
   }

   void BuildArena()
   {
      _arena.reset( new Arena( _arenaDefs, _eventAggregatorMock ) );
      _arena->SetPlayer( _playerMock );
   }

protected:
   shared_ptr<ArenaDefs> _arenaDefs;
   shared_ptr<mock_GameEventAggregator> _eventAggregatorMock;
   shared_ptr<mock_Player> _playerMock;

   shared_ptr<Arena> _arena;
};

TEST_F( ArenaTests, Constructor_Always_SetsDefaultInfoBasedOnConfig )
{
   _arenaDefs->DefaultTiles[5] = { false, true, false, true };
   BuildArena();

   EXPECT_EQ( _arena->GetWidth(), 20 );
   EXPECT_EQ( _arena->GetHeight(), 16 );
   EXPECT_EQ( _arena->GetTileWidth(), 2 );
   EXPECT_EQ( _arena->GetTileHeight(), 2 );
   EXPECT_EQ( _arena->GetHorizontalTiles(), 10 );
   EXPECT_EQ( _arena->GetVerticalTiles(), 8 );

   EXPECT_FALSE( _arena->GetTile( 5 ).LeftPassable );
   EXPECT_TRUE( _arena->GetTile( 5 ).UpPassable );
   EXPECT_FALSE( _arena->GetTile( 5 ).RightPassable );
   EXPECT_TRUE( _arena->GetTile( 5 ).DownPassable );
}

TEST_F( ArenaTests, Reset_Always_ResetsPlayerPosition )
{
   BuildArena();

   Coordinate<long long> position;
   EXPECT_CALL( *_playerMock, SetArenaPosition( _ ) ).WillOnce( SaveArg<0>( &position ) );

   _arena->Reset();

   EXPECT_EQ( position.Left, 10 );
   EXPECT_EQ( position.Top, 8 );
}

TEST_F( ArenaTests, Reset_Always_ClearsEntities )
{
   BuildArena();

   _arena->AddEntity( make_shared<mock_Entity>() );

   EXPECT_EQ( _arena->GetEntityCount(), 2 );

   _arena->Reset();

   EXPECT_EQ( _arena->GetEntityCount(), 1 );
   EXPECT_EQ( _arena->GetEntity( 0 ), _playerMock );
}

TEST_F( ArenaTests, AddEntity_EntityIsNotInList_AddsEntity )
{
   BuildArena();
   auto entityMock = make_shared<mock_Entity>();

   _arena->AddEntity( entityMock );

   EXPECT_EQ( _arena->GetEntityCount(), 2 );
   EXPECT_EQ( _arena->GetEntity( 1 ), entityMock );
}

TEST_F( ArenaTests, AddEntity_EntityIsAlreadyInList_DoesNotAddEntity )
{
   BuildArena();
   auto entityMock = make_shared<mock_Entity>();
   _arena->AddEntity( entityMock );

   _arena->AddEntity( entityMock );

   EXPECT_EQ( _arena->GetEntityCount(), 2 );
   EXPECT_EQ( _arena->GetEntity( 1 ), entityMock );
}

TEST_F( ArenaTests, RemoveEntity_EntityIsNotInList_DoesNotRemoveEntity )
{
   BuildArena();
   auto entityMock1 = make_shared<mock_Entity>();
   auto entityMock2 = make_shared<mock_Entity>();
   _arena->AddEntity( entityMock1 );

   _arena->RemoveEntity( entityMock2 );

   EXPECT_EQ( _arena->GetEntityCount(), 2 );
   EXPECT_EQ( _arena->GetEntity( 1 ), entityMock1 );
}

TEST_F( ArenaTests, AddEntity_EntityIsInList_RemovesEntity )
{
   BuildArena();
   auto entityMock1 = make_shared<mock_Entity>();
   auto entityMock2 = make_shared<mock_Entity>();
   _arena->AddEntity( entityMock1 );
   _arena->AddEntity( entityMock2 );

   _arena->RemoveEntity( entityMock1 );

   EXPECT_EQ( _arena->GetEntityCount(), 2 );
   EXPECT_EQ( _arena->GetEntity( 1 ), entityMock2 );
}
