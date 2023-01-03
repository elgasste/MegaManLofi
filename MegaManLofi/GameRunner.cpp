#include "GameRunner.h"
#include "IGameEventAggregator.h"
#include "IGameClock.h"
#include "IGameInputHandler.h"
#include "IGameRenderer.h"
#include "FrameActionRegistry.h"
#include "IGame.h"
#include "IThread.h"

using namespace MegaManLofi;

GameRunner::GameRunner( const std::shared_ptr<IGameEventAggregator> eventAggregator,
                        const std::shared_ptr<IGameClock> clock,
                        const std::shared_ptr<IGameInputHandler> inputHandler,
                        const std::shared_ptr<IGameRenderer> renderer,
                        const std::shared_ptr<FrameActionRegistry> frameActionRegistry,
                        const std::shared_ptr<IGame> game,
                        const std::shared_ptr<IThread> thread ) :
   _clock( clock ),
   _inputHandler( inputHandler ),
   _renderer( renderer ),
   _frameActionRegistry( frameActionRegistry ),
   _game( game ),
   _isRunning( false )
{
   thread->SetThisThreadToHighestPriority();

   eventAggregator->RegisterEventHandler( GameEvent::Shutdown, std::bind( &GameRunner::HandleShutdownEvent, this ) );
}

void GameRunner::Run()
{
   _isRunning = true;

   while ( _isRunning )
   {
      _clock->StartFrame();

      if ( !_renderer->HasFocus() )
      {
         _inputHandler->HandleInput();
         _game->Tick();
      }

      _renderer->Render();
      _frameActionRegistry->Clear();
      
      _clock->EndFrame();
   }

   _isRunning = false;
}

void GameRunner::HandleShutdownEvent()
{
   _isRunning = false;
}
