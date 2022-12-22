#pragma once

#include <memory>

#include "MenuType.h"

namespace MegaManLofi
{
   class IMenu;

   class __declspec( novtable ) IMenuProvider
   {
   public:
      virtual const std::shared_ptr<IMenu> GetMenu( MenuType type ) const = 0;
   };
}
