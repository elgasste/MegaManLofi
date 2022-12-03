#pragma once

#include <map>
#include <string>

#include "IGameInputConfig.h"

namespace MegaManLofi
{
   enum class KeyCode;
   enum class GameButton;

   class KeyboardInputConfig : public IGameInputConfig
   {
   public:
      std::map<KeyCode, GameButton> KeyMap;

      std::map<KeyCode, std::string> KeyNames;
      std::map<GameButton, std::string> ButtonNames;
   };
}
