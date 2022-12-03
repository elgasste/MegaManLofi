#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/GameInputHandler.h>
#include <MegaManLofi/GameState.h>
#include <MegaManLofi/GameButton.h>
#include <MegaManLofi/GameEvent.h>

#include "mock_GameInputReader.h"
#include "mock_GameInfoProvider.h"
#include "mock_GameEventAggregator.h"
#include "mock_GameInputHandler.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class GameInputHandlerTests : public Test
{
public:
   void SetUp() override
   {
      _inputReaderMock.reset( new NiceMock<mock_GameInputReader> );
      _gameInfoProviderMock.reset( new NiceMock<mock_GameInfoProvider> );
      _eventAggregatorMock.reset( new NiceMock<mock_GameEventAggregator> );
      _startupInputHandlerMock.reset( new NiceMock<mock_GameInputHandler> );

      _inputHandler.reset( new GameInputHandler( _inputReaderMock,
                                                 _gameInfoProviderMock,
                                                 _eventAggregatorMock ) );

      _inputHandler->AddInputHandlerForGameState( GameState::Startup, _startupInputHandlerMock );

      ON_CALL( *_gameInfoProviderMock, GetGameState() ).WillByDefault( Return( GameState::Startup ) );
      ON_CALL( *_inputReaderMock, WasButtonPressed( _ ) ).WillByDefault( Return( false ) );
   }

protected:
   shared_ptr<mock_GameInputReader> _inputReaderMock;
   shared_ptr<mock_GameInfoProvider> _gameInfoProviderMock;
   shared_ptr<mock_GameEventAggregator> _eventAggregatorMock;
   shared_ptr<mock_GameInputHandler> _startupInputHandlerMock;

   shared_ptr<GameInputHandler> _inputHandler;
};

TEST_F( GameInputHandlerTests, HandleInput_Always_ReadsInput )
{
   EXPECT_CALL( *_inputReaderMock, ReadInput() );

   _inputHandler->HandleInput();
}

TEST_F( GameInputHandlerTests, HandleInput_DiagnosticsButtonWasNotPressed_DoesNotRaiseAnyEvents )
{
   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( _ ) ).Times( 0 );

   _inputHandler->HandleInput();
}

TEST_F( GameInputHandlerTests, HandleInput_DiagnosticsButtonWasPressed_RaisesToggleDiagnosticsEvent )
{
   ON_CALL( *_inputReaderMock, WasButtonPressed( GameButton::Diagnostics ) ).WillByDefault( Return( true ) );
   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ToggleDiagnostics ) );

   _inputHandler->HandleInput();
}

TEST_F( GameInputHandlerTests, HandleInput_InputHandlerNotFoundForState_ThrowsException )
{
   ON_CALL( *_gameInfoProviderMock, GetGameState() ).WillByDefault( Return( GameState::Playing ) );

   EXPECT_THROW( _inputHandler->HandleInput(), out_of_range );
}

TEST_F( GameInputHandlerTests, HandleInput_InputHandlerFoundForState_HandlesInputForState )
{
   EXPECT_CALL( *_startupInputHandlerMock, HandleInput() );

   _inputHandler->HandleInput();
}
