#pragma once

#include <string>
#include <vector>

#include "IMenu.h"

namespace MegaManLofi
{
   class PlayingMenu : public IMenu
   {
   public:
      PlayingMenu();

      int GetOptionCount() const override { return (int)_optionTitles.size(); }
      const std::string& GetOptionTitle( int index ) const override { return _optionTitles[index]; }
      int GetSelectedIndex() const override { return _selectedIndex; }
      void ExecuteSelectedIndex() override;

   private:
      std::vector<std::string> _optionTitles;

      int _selectedIndex;
   };
}
