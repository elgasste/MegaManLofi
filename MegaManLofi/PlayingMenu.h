#pragma once

#include <memory>
#include <string>
#include <vector>

#include "IMenu.h"
#include "GameCommand.h"

namespace MegaManLofi
{
   class IGameCommandExecutor;

   class PlayingMenu : public IMenu
   {
   public:
      PlayingMenu( const std::shared_ptr<IGameCommandExecutor> commandExecutor );

      int GetOptionCount() const override { return (int)_optionTitles.size(); }
      const std::string& GetOptionTitle( int index ) const override { return _optionTitles[index]; }
      int GetSelectedIndex() const override { return _selectedIndex; }
      void IncrementSelectedIndex() override;
      void DecrementSelectedIndex() override;
      void ExecuteSelectedIndex() override;

   private:
      const std::shared_ptr<IGameCommandExecutor> _commandExecutor;

      std::vector<std::string> _optionTitles;
      std::vector<GameCommand> _optionCommands;

      int _selectedIndex;
   };
}
