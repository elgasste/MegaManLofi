#include "ArenaImageGenerator.h"
#include "ArenaGenerationStrings.h"
#include "StageDefs.h"
#include "ConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

map<int, vector<int>> ArenaImageGenerator::GenerateArenaTileImageIdMap( const std::shared_ptr<StageDefs> stageDefs )
{
   map<int, vector<int>> tileImageIdMap;

   for ( auto [arenaId, arena] : stageDefs->ArenaMap )
   {
      auto arenaTilesString = ArenaGenerationStrings::GetArenaTilesString( arenaId );

      for ( int i = 0; i < (int)arenaTilesString.size(); i++ )
      {
         switch ( arenaTilesString[i] )
         {
            case 's':
               tileImageIdMap[arenaId].push_back( 0 ); // passable in any direction
               break;
            case 'p':
               tileImageIdMap[arenaId].push_back( 1 ); // passable in any direction but down
               break;
            case 'x':
               tileImageIdMap[arenaId].push_back( 2 ); // passable in any direction but up
               break;
            default:
               tileImageIdMap[arenaId].push_back( -1 );
               break;
         }
      }
   }
   
   return tileImageIdMap;
}

ConsoleImage ArenaImageGenerator::GeneratePauseOverlayImage()
{
   ConsoleImage image;

   image.Width = 28;
   image.Height = 3;

   string content =
      "                            " \
      "          (PAUSED)          " \
      "                            ";

   for ( int i = 0; i < (int)content.size(); i++ )
   {
      image.Pixels.push_back( { content[i], false, ConsoleColor::White, ConsoleColor::DarkMagenta } );
   }

   return image;
}

ConsoleImage ArenaImageGenerator::GenerateGameOverImage()
{
   ConsoleImage image;

   image.Width = 76;
   image.Height = 10;

   string content =
      " _______  _______  __   __  _______     _______  __   __  _______  ______   " \
      "|       ||   _   ||  |_|  ||       |   |       ||  | |  ||       ||    _ |  " \
      "|    ___||  |_|  ||       ||    ___|   |   _   ||  |_|  ||    ___||   | ||  " \
      "|   | __ |       ||       ||   |___    |  | |  ||       ||   |___ |   |_||_ " \
      "|   ||  ||       ||       ||    ___|   |  |_|  ||       ||    ___||    __  |" \
      "|   |_| ||   _   || ||_|| ||   |___    |       | |     | |   |___ |   |  | |" \
      "|_______||__| |__||_|   |_||_______|   |_______|  |___|  |_______||___|  |_|" \
      "                                                                            " \
      "                                                                            " \
      "                        press any button to continue                        ";

   for ( int i = 0; i < (int)content.size(); i++ )
   {
      image.Pixels.push_back( { content[i], true, ConsoleColor::White, ConsoleColor::Black } );
   }

   return image;
}
