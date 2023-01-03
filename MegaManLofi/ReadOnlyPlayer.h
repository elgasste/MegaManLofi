#pragma once

namespace MegaManLofi
{
   class ReadOnlyPlayer
   {
   public:
      ReadOnlyPlayer();

      virtual unsigned int GetLivesRemaining() const { return _livesRemaining; }
      virtual bool IsJumping() const { return _isJumping; }

   protected:
      unsigned int _livesRemaining;
      bool _isJumping;
   };
}
