#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IRandom
   {
   public:
      virtual int GetInt( int min, int max ) const = 0;
   };
}
