#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IRandom
   {
   public:
      virtual unsigned int GetUnsignedInt( unsigned int min, unsigned int max ) const = 0;
   };
}
