#pragma once

#include <memory>

namespace MegaManLofi
{
   class ReadOnlyArena;

   class __declspec( novtable ) IArenaInfoProvider
   {
   public:
      virtual const std::shared_ptr<ReadOnlyArena> GetActiveArena() const = 0;
   };
}
