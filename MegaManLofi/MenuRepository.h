#pragma once

#include <map>

#include "IMenuProvider.h"

namespace MegaManLofi
{
   class MenuRepository : public IMenuProvider
   {
   public:
      void AddMenu( MenuType type, std::shared_ptr<IMenu> menu );

      const std::shared_ptr<IMenu> GetMenu( MenuType type ) const;

   private:
      std::map<MenuType, std::shared_ptr<IMenu>> _menuMap;
   };
}
