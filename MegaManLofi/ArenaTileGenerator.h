#pragma once

#include <vector>

#include "ArenaTile.h"

namespace MegaManLofi
{
   class ArenaTileGenerator
   {
   public:
      static std::vector<ArenaTile> GenerateArenaTiles( int arenaId );
   };
}
