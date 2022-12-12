#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/GameOverStateInputHandler.h>
#include <MegaManLofi/GameButton.h>
#include <MegaManLofi/GameCommand.h>
#include <MegaManLofi/GameCommandArgs.h>

#include "mock_GameInputReader.h"
#include "mock_GameCommandExecutor.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class GameOverStateInputHandlerTests : public Test
{
public:
   void SetUp() override
   {
      _inputReaderMock.reset( new NiceMock<mock_GameInputReader> );
      _commandExecutorMock.reset( new NiceMock<mock_GameCommandExecutor> );

      _inputHandler.reset( new GameOverStateInputHandler( _inputReaderMock,
                                                          _commandExecutorMock ) );
   }

protected:
   shared_ptr<mock_GameInputReader> _inputReaderMock;
   shared_ptr<mock_GameCommandExecutor> _commandExecutorMock;

   shared_ptr<GameOverStateInputHandler> _inputHandler;
};

TEST_F( GameOverStateInputHandlerTests, HandleInput_NoButtonsWerePressed_DoesNotExecuteAnyCommand )
{
   ON_CALL( *_inputReaderMock, WasAnyButtonPressed() ).WillByDefault( Return( false ) );

   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( _, _ ) ).Times( 0 );

   _inputHandler->HandleInput();
}

TEST_F( GameOverStateInputHandlerTests, HandleInput_ButtonWasPressed_ExecutesStartStageCommand )
{
   ON_CALL( *_inputReaderMock, WasAnyButtonPressed() ).WillByDefault( Return( true ) );

   auto args = shared_ptr<GameCommandArgs>();
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::StartStage ) );

   _inputHandler->HandleInput();
}
