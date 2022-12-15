#pragma once

namespace MegaManLofi
{
   enum class GameEvent
   {
      Shutdown = 0,
      ToggleDiagnostics,

      GameStarted,
      StageStarted,

      Pitfall,
      TileDeath
   };
}
