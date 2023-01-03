#pragma once

#include "IReadOnlyArena.h"

namespace MegaManLofi
{
   class IPlayer;
   class IEntity;

   class __declspec( novtable ) IArena : public IReadOnlyArena
   {
   public:
      virtual void Reset() = 0;

      virtual const std::shared_ptr<IPlayer> GetMutablePlayer() const = 0;
      virtual void SetPlayer( const std::shared_ptr<IPlayer> player ) = 0;
      virtual void SetActiveRegion( Quad<float> region ) = 0;

      virtual const std::shared_ptr<IEntity> GetMutableEntity( int index ) const = 0;
      virtual void AddEntity( const std::shared_ptr<IEntity> entity ) = 0;
      virtual void RemoveEntity( const std::shared_ptr<IEntity> entity ) = 0;
   };
}
