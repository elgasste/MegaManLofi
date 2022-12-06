#pragma once

#include <map>

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

      short ArenaCharWidth = 0;
      short ArenaCharHeight = 0;

      short ArenaFenceX = 0;
      short ArenaFenceY = 0;

      double GameStartSingleBlinkSeconds = 0;
      int GameStartBlinkCount = 0;

      ConsoleColor DefaultForegroundColor = (ConsoleColor)0;
      ConsoleColor DefaultBackgroundColor = (ConsoleColor)0;

      std::map<Direction, ConsoleSprite> PlayerStaticSpriteMap;
      std::map<Direction, ConsoleSprite> PlayerMovingSpriteMap;

      std::map<int, ConsoleSprite> ArenaSprites;
      std::map<int, int> ArenaSpriteMap;
   };
}
