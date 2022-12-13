#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/GameRenderer.h>
#include <MegaManLofi/ConsoleRenderConfig.h>
#include <MegaManLofi/IScreenBuffer.h>
#include <MegaManLofi/IGameInfoProvider.h>
#include <MegaManLofi/GameEventAggregator.h>
#include <MegaManLofi/ConsoleColor.h>
#include <MegaManLofi/GameState.h>
#include <MegaManLofi/ConsoleSprite.h>

#include "mock_ScreenBuffer.h"
#include "mock_GameInfoProvider.h"
#include "mock_GameRenderer.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class GameRendererTests : public Test
{
public:
   void SetUp() override
   {
      _renderConfig.reset( new ConsoleRenderConfig );
      _screenBufferMock.reset( new NiceMock<mock_ScreenBuffer> );
      _gameInfoProviderMock.reset( new NiceMock<mock_GameInfoProvider> );
      _diagnosticsRendererMock.reset( new NiceMock<mock_GameRenderer> );
      _eventAggregator.reset( new GameEventAggregator );
      _startupStateRendererMock.reset( new NiceMock<mock_GameRenderer> );

      _renderConfig->DefaultBackgroundColor = ConsoleColor::Black;
      _renderConfig->DefaultForegroundColor = ConsoleColor::Grey;

      ON_CALL( *_gameInfoProviderMock, GetGameState() ).WillByDefault( Return( GameState::Title ) );
      ON_CALL( *_startupStateRendererMock, HasFocus() ).WillByDefault( Return( false ) );
   }

   void BuildRenderer()
   {
      _renderer.reset( new GameRenderer( _renderConfig,
                                         _screenBufferMock,
                                         _gameInfoProviderMock,
                                         _diagnosticsRendererMock,
                                         _eventAggregator ) );

      _renderer->AddRendererForGameState( GameState::Title, _startupStateRendererMock );
   }

protected:
   shared_ptr<ConsoleRenderConfig> _renderConfig;
   shared_ptr<mock_ScreenBuffer> _screenBufferMock;
   shared_ptr<mock_GameInfoProvider> _gameInfoProviderMock;
   shared_ptr<mock_GameRenderer> _diagnosticsRendererMock;
   shared_ptr<GameEventAggregator> _eventAggregator;
   shared_ptr<mock_GameRenderer> _startupStateRendererMock;

   shared_ptr<GameRenderer> _renderer;
};

TEST_F( GameRendererTests, Constructor_Always_InitializesScreenBuffer )
{
   auto baseConfig = static_pointer_cast<IGameRenderConfig>( _renderConfig );
   EXPECT_CALL( *_screenBufferMock, LoadRenderConfig( baseConfig ) );

   BuildRenderer();
}

TEST_F( GameRendererTests, Render_IsCleaningUp_DoesNotRenderAnything )
{
   BuildRenderer();

   _eventAggregator->RaiseEvent( GameEvent::Shutdown );

   EXPECT_CALL( *_screenBufferMock, Clear() ).Times( 0 );
   EXPECT_CALL( *_startupStateRendererMock, Render() ).Times( 0 );
   EXPECT_CALL( *_screenBufferMock, Flip() ).Times( 0 );

   _renderer->Render();
}

TEST_F( GameRendererTests, Render_IsNotCleaningUp_ClearsConsoleBuffer )
{
   BuildRenderer();

   EXPECT_CALL( *_screenBufferMock, Clear() );

   _renderer->Render();
}

TEST_F( GameRendererTests, Render_RendererNotFoundForState_ThrowsException )
{
   BuildRenderer();

   ON_CALL( *_gameInfoProviderMock, GetGameState() ).WillByDefault( Return( GameState::Playing ) );

   EXPECT_THROW( _renderer->Render(), out_of_range );
}

TEST_F( GameRendererTests, Render_RendererFoundForState_RendersState )
{
   BuildRenderer();

   EXPECT_CALL( *_startupStateRendererMock, Render() );

   _renderer->Render();
}

TEST_F( GameRendererTests, Render_StateWasRendered_FlipsConsoleBuffer )
{
   BuildRenderer();

   EXPECT_CALL( *_screenBufferMock, Flip() );

   _renderer->Render();
}

TEST_F( GameRendererTests, HasFocus_StateRendererDoesNotHaveFocus_ReturnsFalse )
{
   BuildRenderer();

   EXPECT_FALSE( _renderer->HasFocus() );
}

TEST_F( GameRendererTests, HasFocus_StateRendererHasFocus_ReturnsTrue )
{
   BuildRenderer();

   EXPECT_CALL( *_startupStateRendererMock, HasFocus() ).WillOnce( Return( true ) );

   EXPECT_TRUE( _renderer->HasFocus() );
}

TEST_F( GameRendererTests, ShutdownEventRaised_Always_CleansUpConsoleBuffer )
{
   BuildRenderer();

   EXPECT_CALL( *_screenBufferMock, CleanUp() );

   _eventAggregator->RaiseEvent( GameEvent::Shutdown );
}

TEST_F( GameRendererTests, ToggleDiagnosticsEventRaised_DiagnosticsNotShowing_RendersDiagnostics )
{
   BuildRenderer();

   EXPECT_CALL( *_diagnosticsRendererMock, Render() );

   _eventAggregator->RaiseEvent( GameEvent::ToggleDiagnostics );

   _renderer->Render();
}

TEST_F( GameRendererTests, ToggleDiagnosticsEventRaised_DiagnosticsShowing_DoesNotRenderDiagnostics )
{
   BuildRenderer();

   _eventAggregator->RaiseEvent( GameEvent::ToggleDiagnostics );
   _renderer->Render();
   _eventAggregator->RaiseEvent( GameEvent::ToggleDiagnostics );

   EXPECT_CALL( *_diagnosticsRendererMock, Render() ).Times( 0 );

   _renderer->Render();
}
