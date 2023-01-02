#pragma once

#include <memory>

#include "IGameClock.h"

namespace MegaManLofi
{
   class IHighResolutionClock;

   class GameClock : public IGameClock
   {
   public:
      GameClock( const std::shared_ptr<IHighResolutionClock> highResolutionClock );

      long long GetMinimumFrameRate() const override { return _minimumFrameRate; }
      void SetMinimumFrameRate( long long frameRate ) override;
      bool HasMinimumFrameRate() const override { return _hasMinimumFrameRate; }

      void StartFrame() override;
      void EndFrame() override;

      long long GetElapsedNanoseconds() const override { return _totalDurationNano; }
      long long GetCurrentFrame() const override { return _totalFrameCount; }
      long long GetAverageFrameRate() const override;
      double GetFrameSeconds() const override;

   private:
      const std::shared_ptr<IHighResolutionClock> _highResolutionClock;

      long long _minimumFrameRate;
      double _minSecondsPerFrame;
      bool _hasMinimumFrameRate;

      long long _totalFrameCount;
      long long _absoluteStartTimeNano;
      long long _frameStartTimeNano;
      long long _lastFrameDurationNano;
      long long _totalDurationNano;
   };
}
