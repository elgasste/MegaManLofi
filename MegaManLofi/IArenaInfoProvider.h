#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IArenaInfoProvider
   {
   public:
      virtual double GetWidth() const = 0;
      virtual double GetHeight() const = 0;

      virtual double GetPlayerPositionX() const = 0;
      virtual double GetPlayerPositionY() const = 0;
   };
}
