#pragma once

#include <memory>

#include "ReadOnlyPlayer.h"
#include "Entity.h"

namespace MegaManLofi
{
   class PlayerDefs;
   class FrameActionRegistry;
   class IFrameRateProvider;
   class GameEventAggregator;

   class Player : public ReadOnlyPlayer,
                  public Entity
   {
   public:
      Player() { }
      Player( const std::shared_ptr<PlayerDefs> playerDefs,
              const std::shared_ptr<FrameActionRegistry> frameActionRegistry,
              const std::shared_ptr<IFrameRateProvider> frameRateProvider,
              const std::shared_ptr<GameEventAggregator> eventAggregator );

      virtual void Reset();
      virtual void ResetPosition();

      virtual void SetLivesRemaining( unsigned int lives ) { _livesRemaining = lives; };
      virtual void PointTo( Direction direction ) { _direction = direction; }
      virtual void PushTo( Direction direction );
      virtual void Jump();
      virtual void ExtendJump();

      virtual void StopY() override;
      virtual bool TakeCollisionPayload( const EntityCollisionPayload& payload ) override;

   private:
      const std::shared_ptr<PlayerDefs> _playerDefs;
      const std::shared_ptr<FrameActionRegistry> _frameActionRegistry;
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<GameEventAggregator> _eventAggregator;
   };
}
