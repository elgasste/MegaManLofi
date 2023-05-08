#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IRandom
   {
   public:
      virtual float GetFloat( float min, float max ) const = 0;
      virtual int GetInt( int min, int max ) const = 0;
   };
}
