#pragma once

namespace MegaManLofi
{
   enum class GameEvent
   {
      Shutdown = 0,
      ToggleDiagnostics,

      StageStarted,

      Pitfall,
      TileDeath
   };
}
