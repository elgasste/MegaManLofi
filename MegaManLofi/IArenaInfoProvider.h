#pragma once

#include <memory>

namespace MegaManLofi
{
   class IReadOnlyArena;

   class __declspec( novtable ) IArenaInfoProvider
   {
   public:
      virtual const std::shared_ptr<IReadOnlyArena> GetArena() const = 0;
   };
}
