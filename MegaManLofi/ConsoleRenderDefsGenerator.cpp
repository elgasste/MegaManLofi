#include "ConsoleRenderDefsGenerator.h"
#include "ConsoleRenderDefs.h"
#include "IFrameRateProvider.h"
#include "TitleImageGenerator.h"
#include "ArenaImageGenerator.h"
#include "MenuImageGenerator.h"
#include "ConsoleSpriteDefsGenerator.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<IGameRenderDefs> ConsoleRenderDefsGenerator::GenerateConsoleRenderDefs( const shared_ptr<IFrameRateProvider> frameRateProvider,
                                                                                   const shared_ptr<StageDefs> stageDefs )
{
   auto renderDefs = make_shared<ConsoleRenderDefs>();

   renderDefs->ConsoleWidthChars = 120;
   renderDefs->ConsoleHeightChars = 30;

   renderDefs->ArenaViewportLeftChars = 0;
   renderDefs->ArenaViewportTopChars = 2;

   renderDefs->ArenaCharWidth = 38;
   renderDefs->ArenaCharHeight = 78;

   renderDefs->ArenaViewportWidthChars = 120;
   renderDefs->ArenaViewportHeightChars = 28;

   renderDefs->ArenaStatusBarLeftChars = 0;
   renderDefs->ArenaStatusBarTopChars = 0;
   renderDefs->ArenaStatusBarWidthChars = 120;

   renderDefs->PitfallAnimationSeconds = 2;

   // player explosion should swap between two sprites in 1/4 second increments
   renderDefs->PlayerExplosionAnimationSeconds = 3;
   renderDefs->PlayerExplosionParticleVelocity = 1'000;

   renderDefs->DefaultForegroundColor = ConsoleColor::Grey;
   renderDefs->DefaultBackgroundColor = ConsoleColor::Black;

   renderDefs->TitleScreenForegroundColor = ConsoleColor::White;
   renderDefs->TitleScreenBackgroundColor = ConsoleColor::Black;

   renderDefs->ArenaForegroundColor = ConsoleColor::White;
   renderDefs->ArenaBackgroundColor = ConsoleColor::Black;

   renderDefs->PlayingMenuForegroundColor = ConsoleColor::White;
   renderDefs->PlayingMenuBackgroundColor = ConsoleColor::DarkBlue;

   renderDefs->GameOverBackgroundColor = ConsoleColor::DarkMagenta;

   renderDefs->TitleKeyBindingsForegroundColor = ConsoleColor::DarkGrey;

   renderDefs->TitleTextImage = TitleImageGenerator::GenerateTitleTextImage();
   renderDefs->TitleSubTextImage = TitleImageGenerator::GenerateTitleSubTextImage();
   renderDefs->TitlePlayerImage = TitleImageGenerator::GeneratePlayerImage();
   renderDefs->TitleBuildingImage = TitleImageGenerator::GenerateBuildingImage();
   renderDefs->TitleStartMessageImage = TitleImageGenerator::GenerateStartMessageImage();
   renderDefs->TitleStarImage = TitleImageGenerator::GenerateStarImage();

   renderDefs->PlayerThwipVelocity = 5'000;
   renderDefs->PlayerPostThwipDelaySeconds = 1;

   renderDefs->TitleTextLeftChars = 6;
   renderDefs->TitleTextTopChars = 1;
   renderDefs->TitleSubTextLeftChars = renderDefs->TitleTextLeftChars + renderDefs->TitleTextImage.Width;
   renderDefs->TitleSubTextTopChars = renderDefs->TitleTextTopChars;
   renderDefs->TitleBuildingLeftChars = renderDefs->ConsoleWidthChars - renderDefs->TitleBuildingImage.Width - 1;
   renderDefs->TitleBuildingTopChars = renderDefs->ConsoleHeightChars - renderDefs->TitleBuildingImage.Height - 1;
   renderDefs->TitlePlayerLeftChars = renderDefs->TitleBuildingLeftChars + 6;
   renderDefs->TitlePlayerTopChars = renderDefs->TitleBuildingTopChars - renderDefs->TitlePlayerImage.Height;
   renderDefs->TitleStartMessageLeftChars = 55;
   renderDefs->TitleStartMessageTopChars = 15;
   renderDefs->TitleKeyBindingsMiddleXChars = 25;
   renderDefs->TitleKeyBindingsTopChars = renderDefs->TitleTextTopChars + renderDefs->TitleTextImage.Height + 3;
   renderDefs->TitleStarCount = 20;
   renderDefs->MinTitleStarVelocity = 500;
   renderDefs->MaxTitleStarVelocity = 1'500;

   renderDefs->GetReadyAnimationSeconds = 2;

   renderDefs->PauseOverlayImage = ArenaImageGenerator::GeneratePauseOverlayImage();
   renderDefs->GameOverImage = ArenaImageGenerator::GenerateGameOverImage();

   renderDefs->PlayingMenuPlayerImage = MenuImageGenerator::GeneratePlayerImage();

   // TODO: move these into a sub-generator

   // ground that is impassable in all directions
   renderDefs->ArenaImageMap[0].Width = 1;
   renderDefs->ArenaImageMap[0].Height = 1;
   renderDefs->ArenaImageMap[0].Pixels.push_back( { 'Z', true, ConsoleColor::DarkYellow, ConsoleColor::Black } );

   // ground that is only impassable downward
   renderDefs->ArenaImageMap[1].Width = 1;
   renderDefs->ArenaImageMap[1].Height = 1;
   renderDefs->ArenaImageMap[1].Pixels.push_back( { '-', true, ConsoleColor::DarkYellow, ConsoleColor::Black } );

   // spike that is only impassable upward
   renderDefs->ArenaImageMap[2].Width = 1;
   renderDefs->ArenaImageMap[2].Height = 1;
   renderDefs->ArenaImageMap[2].Pixels.push_back( { '*', true, ConsoleColor::Red, ConsoleColor::Black } );

   renderDefs->ArenaTileImageIdMap = ArenaImageGenerator::GenerateArenaTileImageIdMap( stageDefs );

   renderDefs->SpriteDefs = ConsoleSpriteDefsGenerator::GenerateConsoleSpriteDefs( frameRateProvider );

   return renderDefs;
}
