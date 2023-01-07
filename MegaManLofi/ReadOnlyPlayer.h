#pragma once

namespace MegaManLofi
{
   class ReadOnlyPlayer
   {
   public:
      ReadOnlyPlayer();

      virtual unsigned int GetLivesRemaining() const { return _livesRemaining; }
      virtual bool IsJumping() const { return _isJumping; }
      virtual float GetMaxPushVelocity() const { return _maxPushVelocity; }
      virtual float GetPushAccelerationPerSecond() const { return _pushAccelerationPerSecond; }
      virtual float GetJumpAccelerationPerSecond() const { return _jumpAccelerationPerSecond; }
      virtual float GetMaxJumpExtensionSeconds() const { return _maxJumpExtensionSeconds; }

   protected:
      unsigned int _livesRemaining;
      bool _isJumping;
      float _maxPushVelocity;
      float _pushAccelerationPerSecond;
      float _jumpAccelerationPerSecond;
      float _maxJumpExtensionSeconds;
   };
}
