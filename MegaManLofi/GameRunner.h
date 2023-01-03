#pragma once

#include <memory>

#include "IGameRunner.h"

namespace MegaManLofi
{
   class IGameEventAggregator;
   class IGameClock;
   class IGameInputHandler;
   class IGameRenderer;
   class FrameActionRegistry;
   class IGame;
   class IThread;

   class GameRunner : public IGameRunner
   {
   public:
      GameRunner( const std::shared_ptr<IGameEventAggregator> eventAggregator,
                  const std::shared_ptr<IGameClock> clock,
                  const std::shared_ptr<IGameInputHandler> inputHandler,
                  const std::shared_ptr<IGameRenderer> renderer,
                  const std::shared_ptr<FrameActionRegistry> frameActionRegistry,
                  const std::shared_ptr<IGame> game,
                  const std::shared_ptr<IThread> thread );

      void Run() override;

   private:
      void HandleShutdownEvent();

   private:
      const std::shared_ptr<IGameClock> _clock;
      const std::shared_ptr<IGameInputHandler> _inputHandler;
      const std::shared_ptr<IGameRenderer> _renderer;
      const std::shared_ptr<FrameActionRegistry> _frameActionRegistry;
      const std::shared_ptr<IGame> _game;

      bool _isRunning;
   };
}
