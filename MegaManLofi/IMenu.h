#pragma once

#include <string>

namespace MegaManLofi
{
   class __declspec( novtable ) IMenu
   {
   public:
      virtual int GetOptionCount() const = 0;
      virtual const std::string& GetOptionTitle( int index ) const = 0;
      virtual int GetSelectedIndex() const = 0;
      virtual void IncrementSelectedIndex() = 0;
      virtual void DecrementSelectedIndex() = 0;
      virtual void ExecuteSelectedIndex() = 0;
   };
}
