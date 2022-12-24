#pragma once

#include <memory>

#include "IGameClock.h"
#include "IFrameRateProvider.h"

namespace MegaManLofi
{
   class IHighResolutionClock;
   class ISleeper;

   class GameClock : public IGameClock,
                     public IFrameRateProvider
   {
   public:
      GameClock( const std::shared_ptr<IHighResolutionClock> highResolutionClock,
                 const std::shared_ptr<ISleeper> sleeper,
                 unsigned int framesPerSecond );

      void StartFrame() override;
      void WaitForNextFrame() override;

      unsigned int GetFramesPerSecond() const override { return _framesPerSecond; }
      long long GetTotalFrameCount() const override { return _totalFrameCount; }
      long long GetLagFrameCount() const override { return _lagFrameCount; }

      long long GetCurrentFrame() const override { return _totalFrameCount; }
      double GetSecondsPerFrame() const override { return _secondsPerFrame; }

   private:
      const std::shared_ptr<IHighResolutionClock> _highResolutionClock;
      const std::shared_ptr<ISleeper> _sleeper;

      unsigned int _framesPerSecond;
      long long _totalFrameCount;
      long long _lagFrameCount;
      long long _frameStartTimeNano;
      long long _nanoSecondsPerFrame;
      double _secondsPerFrame;
   };
}
