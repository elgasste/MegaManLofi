#pragma once

#include <memory>

#include "IReadOnlyArena.h"
#include "Quad.h"

namespace MegaManLofi
{
   class IPlayer;

   class __declspec( novtable ) IArena : public IReadOnlyArena
   {
   public:
      virtual void Reset() = 0;

      virtual const std::shared_ptr<IPlayer> GetPlayer() const = 0;
      virtual void SetPlayer( const std::shared_ptr<IPlayer> player ) = 0;

      virtual const Quad<long long>& GetActiveRegion() const = 0;
      virtual void SetActiveRegion( Quad<long long> region ) = 0;
   };
}
