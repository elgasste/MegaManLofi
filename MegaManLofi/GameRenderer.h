#pragma once

#include <memory>
#include <map>

#include "IGameRenderer.h"
#include "GameState.h"
#include "GameEvent.h"

namespace MegaManLofi
{
   class ConsoleRenderDefs;
   class IScreenBuffer;
   class IGameInfoProvider;
   class IGameEventAggregator;

   class GameRenderer : public IGameRenderer
   {
   public:
      GameRenderer( const std::shared_ptr<ConsoleRenderDefs> renderDefs,
                    const std::shared_ptr<IScreenBuffer> screenBuffer,
                    const std::shared_ptr<IGameInfoProvider> gameInfoProvider,
                    const std::shared_ptr<IGameRenderer> diagnosticsRenderer,
                    const std::shared_ptr<IGameEventAggregator> eventAggregator );

      void AddRendererForGameState( GameState state, std::shared_ptr<IGameRenderer> renderer );

      void Render() override;
      bool HasFocus() const override;

   private:
      void HandleShutdownEvent();
      void HandleToggleDiagnosticsEvent();

   private:
      const std::shared_ptr<IScreenBuffer> _screenBuffer;
      const std::shared_ptr<IGameInfoProvider> _gameInfoProvider;
      const std::shared_ptr<IGameRenderer> _diagnosticsRenderer;

      std::map<GameState, std::shared_ptr<IGameRenderer>> _stateRenderers;
      std::shared_ptr<IGameRenderer> _stateRendererCache;

      bool _showDiagnostics;
      bool _isCleaningUp;
   };
}
