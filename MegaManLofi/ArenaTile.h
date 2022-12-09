#pragma once

namespace MegaManLofi
{
   struct ArenaTile
   {
      bool LeftPassable = false;
      bool UpPassable = false;
      bool RightPassable = false;
      bool DownPassable = false;

      bool CausesDeath = false;
   };
}
