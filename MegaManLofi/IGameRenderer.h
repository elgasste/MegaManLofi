#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IGameRenderer
   {
   public:
      virtual void Render() = 0;
      virtual bool HasFocus() const = 0;
   };
}
