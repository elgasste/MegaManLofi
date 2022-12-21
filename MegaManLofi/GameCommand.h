#pragma once

namespace MegaManLofi
{
   enum class GameCommand
   {
      StartGame = 0,
      StartStage,
      PushPlayer,
      PointPlayer,
      Jump,
      ExtendJump,
      TogglePause,
      OpenPlayingMenu,
      ExitToTitle,
      Quit
   };
}
