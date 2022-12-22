#include "MenuRepository.h"

using namespace std;
using namespace MegaManLofi;

void MenuRepository::AddMenu( MenuType type, const shared_ptr<IMenu> menu )
{
   _menuMap[type] = menu;
}

const shared_ptr<IMenu> MenuRepository::GetMenu( MenuType type ) const
{
   return _menuMap.at( type );
}
