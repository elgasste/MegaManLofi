#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IGameClock
   {
   public:
      virtual void StartFrame() = 0;
      virtual void WaitForNextFrame() = 0;

      virtual long long GetTotalFrameCount() const = 0;
      virtual long long GetLagFrameCount() const = 0;

      virtual unsigned int GetFramesPerSecond() const = 0;
      virtual long long GetCurrentFrame() const = 0;
      virtual double GetSecondsPerFrame() const = 0;
   };
}
