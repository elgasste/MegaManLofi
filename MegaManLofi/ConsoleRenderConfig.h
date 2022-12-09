#pragma once

#include <map>
#include <vector>

#include "IGameRenderConfig.h"
#include "ConsoleSprite.h"

namespace MegaManLofi
{
   enum class ConsoleColor;
   enum class Direction;

   class ConsoleRenderConfig : public IGameRenderConfig
   {
   public:
      short ConsoleWidth = 0;
      short ConsoleHeight = 0;

      short ArenaViewportX = 0;
      short ArenaViewportY = 0;

      long long ArenaCharWidth = 0;
      long long ArenaCharHeight = 0;

      short ArenaViewportWidthChar = 0;
      short ArenaViewportHeightChar = 0;

      double GameStartSingleBlinkSeconds = 0;
      int GameStartBlinkCount = 0;

      double PitfallAnimationSeconds = 0;

      double PlayerExplosionAnimationSeconds = 0;
      double PlayerExplosionSpriteSwapSeconds = 0;
      ConsoleSprite PlayerExplosionParticleSprite1;
      ConsoleSprite PlayerExplosionParticleSprite2;
      long long PlayerExplosionParticleVelocity = 0;

      ConsoleColor DefaultForegroundColor = (ConsoleColor)0;
      ConsoleColor DefaultBackgroundColor = (ConsoleColor)0;

      ConsoleColor TitleScreenForegroundColor = (ConsoleColor)0;
      ConsoleColor TitleScreenBackgroundColor = (ConsoleColor)0;

      ConsoleColor ArenaForegroundColor = (ConsoleColor)0;
      ConsoleColor ArenaBackgroundColor = (ConsoleColor)0;

      ConsoleColor GameOverBackgroundColor = (ConsoleColor)0;

      ConsoleColor TitleKeyBindingsForegroundColor = (ConsoleColor)0;

      ConsoleSprite TitleTextSprite;
      ConsoleSprite TitleSubTextSprite;
      ConsoleSprite TitlePlayerSprite;
      ConsoleSprite TitleBuildingSprite;
      ConsoleSprite TitleStartMessageSprite;

      short TitleTextX = 0;
      short TitleTextY = 0;
      short TitleSubTextX = 0;
      short TitleSubTextY = 0;
      short TitlePlayerX = 0;
      short TitlePlayerY = 0;
      short TitleBuildingX = 0;
      short TitleBuildingY = 0;
      short TitleStartMessageX = 0;
      short TitleStartMessageY = 0;
      short TitleKeyBindingsMiddleX = 0;
      short TitleKeyBindingsY = 0;

      ConsoleSprite GetReadySprite;
      ConsoleSprite PauseOverlaySprite;
      ConsoleSprite GameOverSprite;

      std::map<Direction, ConsoleSprite> PlayerStaticSpriteMap;
      std::map<Direction, ConsoleSprite> PlayerMovingSpriteMap;

      std::map<int, ConsoleSprite> ArenaSpriteMap;
      std::vector<int> ArenaSprites;
   };
}
