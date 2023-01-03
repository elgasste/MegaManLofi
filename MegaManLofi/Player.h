#pragma once

#include <memory>

#include "ReadOnlyPlayer.h"
#include "Entity.h"

namespace MegaManLofi
{
   class PlayerDefs;
   class IFrameActionRegistry;
   class IFrameRateProvider;

   class Player : public ReadOnlyPlayer,
                  public Entity
   {
   public:
      Player() { }
      Player( const std::shared_ptr<PlayerDefs> playerDefs,
              const std::shared_ptr<IFrameActionRegistry> frameActionRegistry,
              const std::shared_ptr<IFrameRateProvider> frameRateProvider );

      virtual void Reset();
      virtual void ResetPhysics();

      virtual void SetLivesRemaining( unsigned int lives ) { _livesRemaining = lives; };
      virtual void SetIsJumping( bool isJumping ) { _isJumping = isJumping; }

      virtual void StopY() override;

   private:
      const std::shared_ptr<PlayerDefs> _playerDefs;
      const std::shared_ptr<IFrameActionRegistry> _frameActionRegistry;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
   };
}
