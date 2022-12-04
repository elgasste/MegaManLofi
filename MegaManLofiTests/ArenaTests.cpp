#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/Arena.h>
#include <MegaManLofi/ArenaConfig.h>
#include <MegaManLofi/FrameAction.h>
#include <MegaManLofi/Rectangle.h>

#include "mock_Player.h"
#include "mock_FrameActionRegistry.h"
#include "mock_FrameRateProvider.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class ArenaTests : public Test
{
public:
   void SetUp() override
   {
      _config.reset( new ArenaConfig );

      _config->TileWidth = 2.;
      _config->TileHeight = 2.;
      _config->HorizontalTiles = 10;
      _config->VerticalTiles = 8;
      _config->PlayerStartX = 10.;
      _config->PlayerStartY = 8.;

      for ( int i = 0; i < _config->HorizontalTiles * _config->VerticalTiles; i++ )
      {
         _config->Tiles.push_back( { true, true, true, true } );
      }
   }

   void BuildArena()
   {
      _arena.reset( new Arena( _config ) );
   }

protected:
   shared_ptr<ArenaConfig> _config;

   shared_ptr<Arena> _arena;
};

TEST_F( ArenaTests, Constructor_Always_SetsDefaultInfoBasedOnConfig )
{
   _config->Tiles[5] = { false, true, false, true };
   BuildArena();

   EXPECT_EQ( _arena->GetWidth(), 20. );
   EXPECT_EQ( _arena->GetHeight(), 16. );
   EXPECT_EQ( _arena->GetPlayerPositionX(), 10. );
   EXPECT_EQ( _arena->GetPlayerPositionY(), 8. );
   EXPECT_EQ( _arena->GetTileWidth(), 2. );
   EXPECT_EQ( _arena->GetTileHeight(), 2. );
   EXPECT_EQ( _arena->GetHorizontalTiles(), 10 );
   EXPECT_EQ( _arena->GetVerticalTiles(), 8. );

   EXPECT_FALSE( _arena->GetTile( 5 ).LeftPassable );
   EXPECT_TRUE( _arena->GetTile( 5 ).UpPassable );
   EXPECT_FALSE( _arena->GetTile( 5 ).RightPassable );
   EXPECT_TRUE( _arena->GetTile( 5 ).DownPassable );
}
