#pragma once

#include <memory>

#include "MbcVirtualMachine.h"

namespace MegaManLofi
{
   class IFrameRateProvider;

   class EnemyBehavior : public MbcVirtualMachine
   {
   public:
      EnemyBehavior( const std::shared_ptr<IFrameRateProvider> frameRateProvider );

   private:
      bool HandleCommand( mbc_command command ) override;

      void GetFrameSeconds();

   private:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
   };
}
