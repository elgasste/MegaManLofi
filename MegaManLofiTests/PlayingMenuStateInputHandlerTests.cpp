#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/PlayingMenuStateInputHandler.h>
#include <MegaManLofi/GameButton.h>
#include <MegaManLofi/GameCommand.h>

#include "mock_GameInputReader.h"
#include "mock_GameCommandExecutor.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class PlayingMenuStateInputHandlerTests : public Test
{
public:
   void SetUp() override
   {
      _inputReaderMock.reset( new NiceMock<mock_GameInputReader> );
      _commandExecutorMock.reset( new NiceMock<mock_GameCommandExecutor> );

      _inputHandler.reset( new PlayingMenuStateInputHandler( _inputReaderMock,
                                                             _commandExecutorMock ) );

      ON_CALL( *_inputReaderMock, WasButtonPressed( _ ) ).WillByDefault( Return( false ) );
      ON_CALL( *_inputReaderMock, IsButtonDown( _ ) ).WillByDefault( Return( false ) );
   }

protected:
   shared_ptr<mock_GameInputReader> _inputReaderMock;
   shared_ptr<mock_GameCommandExecutor> _commandExecutorMock;

   shared_ptr<PlayingMenuStateInputHandler> _inputHandler;
};

TEST_F( PlayingMenuStateInputHandlerTests, HandleInput_StartButtonWasPressed_ExecutesClosePlayingMenuCommand )
{
   ON_CALL( *_inputReaderMock, WasButtonPressed( GameButton::Start ) ).WillByDefault( Return( true ) );
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::ClosePlayingMenu ) );

   _inputHandler->HandleInput();
}

TEST_F( PlayingMenuStateInputHandlerTests, HandleInput_SelectButtonWasPressed_ExecutesQuitCommand )
{
   ON_CALL( *_inputReaderMock, WasButtonPressed( GameButton::Select ) ).WillByDefault( Return( true ) );
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::Quit ) );

   _inputHandler->HandleInput();
}
