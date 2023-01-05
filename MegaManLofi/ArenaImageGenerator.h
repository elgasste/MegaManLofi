#pragma once

#include <memory>
#include <vector>
#include <map>

#include "ConsoleImage.h"

namespace MegaManLofi
{
   class StageDefs;

   class ArenaImageGenerator
   {
   public:
      static std::map<int, std::vector<int>> GenerateArenaTileImageIdMap( const std::shared_ptr<StageDefs> stageDefs );
      static ConsoleImage GeneratePauseOverlayImage();
      static ConsoleImage GenerateGameOverImage();
   };
}
