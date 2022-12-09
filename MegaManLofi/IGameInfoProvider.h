#pragma once

namespace MegaManLofi
{
   enum class GameState;
   enum class Direction;

   class __declspec( novtable ) IGameInfoProvider
   {
   public:
      virtual GameState GetGameState() const = 0;
      virtual bool IsPaused() const = 0;
   };
}
