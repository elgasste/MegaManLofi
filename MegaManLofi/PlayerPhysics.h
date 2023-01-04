#pragma once

#include <memory>

#include "Direction.h"

namespace MegaManLofi
{
   class IFrameRateProvider;
   class FrameActionRegistry;
   class PlayerPhysicsDefs;
   class Player;

   class PlayerPhysics
   {
   public:
      PlayerPhysics() : _lastExtendJumpFrame( 0 ) { }
      PlayerPhysics( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                     const std::shared_ptr<FrameActionRegistry> frameActionRegistry,
                     const std::shared_ptr<PlayerPhysicsDefs> physicsDefs );

      virtual void AssignTo( const std::shared_ptr<Player> player );
      virtual void Reset();
      virtual void Tick();

      virtual void PointTo( Direction direction ) const;
      virtual void PushTo( Direction direction ) const;
      virtual void Jump();
      virtual void ExtendJump();

   private:
      void ApplyFriction() const;
      void ApplyGravity() const;

   private:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<FrameActionRegistry> _frameActionRegistry;
      const std::shared_ptr<PlayerPhysicsDefs> _physicsDefs;
      std::shared_ptr<Player> _player;

      long long _lastExtendJumpFrame;
   };
}
