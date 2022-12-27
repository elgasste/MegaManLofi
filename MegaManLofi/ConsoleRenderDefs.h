#pragma once

#include <map>
#include <vector>

#include "IGameRenderDefs.h"
#include "Direction.h"
#include "ConsoleImage.h"

namespace MegaManLofi
{
   class ConsoleSpriteDefs;

   class ConsoleRenderDefs : public IGameRenderDefs
   {
   public:
      short ConsoleWidthChars = 0;
      short ConsoleHeightChars = 0;

      short ArenaViewportLeftChars = 0;
      short ArenaViewportTopChars = 0;

      long long ArenaCharWidth = 0;
      long long ArenaCharHeight = 0;

      short ArenaViewportWidthChars = 0;
      short ArenaViewportHeightChars = 0;

      short ArenaStatusBarLeftChars = 0;
      short ArenaStatusBarTopChars = 0;
      short ArenaStatusBarWidthChars = 0;

      double PitfallAnimationSeconds = 0;

      double PlayerExplosionAnimationSeconds = 0;      
      long long PlayerExplosionParticleVelocity = 0;

      ConsoleColor DefaultForegroundColor = (ConsoleColor)0;
      ConsoleColor DefaultBackgroundColor = (ConsoleColor)0;

      ConsoleColor TitleScreenForegroundColor = (ConsoleColor)0;
      ConsoleColor TitleScreenBackgroundColor = (ConsoleColor)0;

      ConsoleColor ArenaForegroundColor = (ConsoleColor)0;
      ConsoleColor ArenaBackgroundColor = (ConsoleColor)0;

      ConsoleColor PlayingMenuForegroundColor = (ConsoleColor)0;
      ConsoleColor PlayingMenuBackgroundColor = (ConsoleColor)0;

      ConsoleColor GameOverBackgroundColor = (ConsoleColor)0;

      ConsoleColor TitleKeyBindingsForegroundColor = (ConsoleColor)0;

      ConsoleImage TitleTextImage;
      ConsoleImage TitleSubTextImage;
      ConsoleImage TitlePlayerImage;
      ConsoleImage TitleBuildingImage;
      ConsoleImage TitleStartMessageImage;
      ConsoleImage TitleStarImage;

      ConsoleImage PlayingMenuPlayerImage;

      long long PlayerThwipVelocity = 0;
      double PlayerPostThwipDelaySeconds = 0;

      short TitleTextLeftChars = 0;
      short TitleTextTopChars = 0;
      short TitleSubTextLeftChars = 0;
      short TitleSubTextTopChars = 0;
      short TitlePlayerLeftChars = 0;
      short TitlePlayerTopChars = 0;
      short TitleBuildingLeftChars = 0;
      short TitleBuildingTopChars = 0;
      short TitleStartMessageLeftChars = 0;
      short TitleStartMessageTopChars = 0;
      short TitleKeyBindingsMiddleXChars = 0;
      short TitleKeyBindingsTopChars = 0;
      int TitleStarCount = 0;
      long long MinTitleStarVelocity = 0;
      long long MaxTitleStarVelocity = 0;

      double GetReadyAnimationSeconds = 0;

      ConsoleImage PauseOverlayImage;
      ConsoleImage GameOverImage;

      std::map<int, ConsoleImage> ArenaImageMap;
      std::vector<int> ArenaTiles;

      std::shared_ptr<ConsoleSpriteDefs> SpriteDefs;
   };
}
