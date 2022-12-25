#include "ConsoleRenderDefsGenerator.h"
#include "ConsoleRenderDefs.h"
#include "IFrameRateProvider.h"
#include "PlayerSpriteGenerator.h"
#include "TitleSpriteGenerator.h"
#include "ArenaSpriteGenerator.h"
#include "MenuSpriteGenerator.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<IGameRenderDefs> ConsoleRenderDefsGenerator::GenerateConsoleRenderDefs( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto renderDefs = make_shared<ConsoleRenderDefs>();

   renderDefs->ConsoleWidthChars = 120;
   renderDefs->ConsoleHeightChars = 30;

   renderDefs->ArenaViewportLeftChars = 0;
   renderDefs->ArenaViewportTopChars = 0;

   renderDefs->ArenaCharWidth = 38'000;
   renderDefs->ArenaCharHeight = 78'000;

   renderDefs->ArenaViewportWidthChars = 120;
   renderDefs->ArenaViewportHeightChars = 30;

   renderDefs->ArenaStatusBarLeftChars = renderDefs->ArenaViewportLeftChars;
   renderDefs->ArenaStatusBarTopChars = renderDefs->ArenaViewportTopChars;
   renderDefs->ArenaStatusBarWidthChars = 20;

   renderDefs->PitfallAnimationSeconds = 2;

   // player explosion should swap between two sprites in 1/4 second increments
   renderDefs->PlayerExplosionAnimationSeconds = 3;
   renderDefs->PlayerExplosionParticleVelocity = 1'000'000;
   renderDefs->PlayerExplosionParticleSprite = PlayerSpriteGenerator::GenerateExplosionParticleSprite( frameRateProvider );

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

   renderDefs->TitleTextImage = TitleSpriteGenerator::GenerateTitleTextImage();
   renderDefs->TitleSubTextImage = TitleSpriteGenerator::GenerateTitleSubTextImage();
   renderDefs->TitlePlayerImage = TitleSpriteGenerator::GeneratePlayerImage();
   renderDefs->TitleBuildingImage = TitleSpriteGenerator::GenerateBuildingImage();
   renderDefs->TitleStartMessageImage = TitleSpriteGenerator::GenerateStartMessageImage();
   renderDefs->TitleStarImage = TitleSpriteGenerator::GenerateStarImage();

   renderDefs->PlayerThwipSprite = PlayerSpriteGenerator::GenerateThwipSprite( frameRateProvider );
   renderDefs->PlayerThwipInTransitionSprite = PlayerSpriteGenerator::GenerateThwipInTransitionSprite( frameRateProvider );
   renderDefs->PlayerThwipOutTransitionSprite = PlayerSpriteGenerator::GenerateThwipOutTransitionSprite( frameRateProvider );
   renderDefs->PlayerThwipVelocity = 5'000'000;
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
   renderDefs->MinTitleStarVelocity = 500'000;
   renderDefs->MaxTitleStarVelocity = 1'500'000;

   renderDefs->GetReadySprite = ArenaSpriteGenerator::GenerateGetReadySprite( frameRateProvider );
   renderDefs->GetReadyAnimationSeconds = 2;

   renderDefs->PauseOverlayImage = ArenaSpriteGenerator::GeneratePauseOverlayImage();
   renderDefs->GameOverImage = ArenaSpriteGenerator::GenerateGameOverImage();

   renderDefs->MenuCaratSprite = MenuSpriteGenerator::GenerateMenuCaratSprite( frameRateProvider );
   renderDefs->PlayingMenuPlayerImage = MenuSpriteGenerator::GeneratePlayerImage();

   renderDefs->PlayerStandingSpriteMap = PlayerSpriteGenerator::GenerateStandingSpriteMap( frameRateProvider );
   renderDefs->PlayerWalkingSpriteMap = PlayerSpriteGenerator::GenerateWalkingSpriteMap( frameRateProvider );
   renderDefs->PlayerFallingSpriteMap = PlayerSpriteGenerator::GenerateFallingSpriteMap( frameRateProvider );

   // ground that is impassable in all directions
   renderDefs->ArenaImageMap[0].Width = 1;
   renderDefs->ArenaImageMap[0].Height = 1;
   renderDefs->ArenaImageMap[0].Pixels.push_back( { 'X', true, ConsoleColor::Yellow, ConsoleColor::Black } );

   // ground that is only impassable downward
   renderDefs->ArenaImageMap[1].Width = 1;
   renderDefs->ArenaImageMap[1].Height = 1;
   renderDefs->ArenaImageMap[1].Pixels.push_back( { '-', true, ConsoleColor::Yellow, ConsoleColor::Black } );

   // spike that is only impassable upward
   renderDefs->ArenaImageMap[2].Width = 1;
   renderDefs->ArenaImageMap[2].Height = 1;
   renderDefs->ArenaImageMap[2].Pixels.push_back( { '+', true, ConsoleColor::Red, ConsoleColor::Black } );

   renderDefs->ArenaTiles = ArenaSpriteGenerator::GenerateArenaTiles();

   return renderDefs;
}
