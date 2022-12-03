#pragma once

#include <memory>

namespace MegaManLofi
{
   enum class GameCommand;
   class GameCommandArgs;

   class _declspec( novtable ) IGameCommandExecutor
   {
   public:
      virtual void ExecuteCommand( GameCommand command ) = 0;
      virtual void ExecuteCommand( GameCommand command, const std::shared_ptr<GameCommandArgs> args ) = 0;
   };
}
