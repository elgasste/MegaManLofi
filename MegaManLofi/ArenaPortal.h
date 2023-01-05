#pragma once

namespace MegaManLofi
{
   struct ArenaPortal
   {
      int FromArenaId = 0;
      int ToArenaId = 0;

      // If the portal is on the left or right, this is the location of the portal from the
      // top of the arena. If the portal is on the top or bottom, this will be from the left.
      int TileOffset = 0;

      // Basically the size of the portal, in tiles
      int TileRange = 0;
   };
}
