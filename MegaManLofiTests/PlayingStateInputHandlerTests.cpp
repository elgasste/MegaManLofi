#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/PlayingStateInputHandler.h>
#include <MegaManLofi/GameButton.h>
#include <MegaManLofi/GameCommand.h>
#include <MegaManLofi/PushPlayerCommandArgs.h>
#include <MegaManLofi/PointPlayerCommandArgs.h>
#include <MegaManLofi/Direction.h>

#include "mock_GameInputReader.h"
#include "mock_GameCommandExecutor.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class PlayingStateInputHandlerTests : public Test
{
public:
   void SetUp() override
   {
      _inputReaderMock.reset( new NiceMock<mock_GameInputReader> );
      _commandExecutorMock.reset( new NiceMock<mock_GameCommandExecutor> );

      _inputHandler.reset( new PlayingStateInputHandler( _inputReaderMock,
                                                         _commandExecutorMock ) );

      ON_CALL( *_inputReaderMock, WasButtonPressed( _ ) ).WillByDefault( Return( false ) );
      ON_CALL( *_inputReaderMock, IsButtonDown( _ ) ).WillByDefault( Return( false ) );
   }

protected:
   shared_ptr<mock_GameInputReader> _inputReaderMock;
   shared_ptr<mock_GameCommandExecutor> _commandExecutorMock;

   shared_ptr<PlayingStateInputHandler> _inputHandler;
};

TEST_F( PlayingStateInputHandlerTests, HandleInput_SelectButtonWasPressed_ExecutesTogglePauseCommand )
{
   ON_CALL( *_inputReaderMock, WasButtonPressed( GameButton::Select ) ).WillByDefault( Return( true ) );
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::TogglePause ) );

   _inputHandler->HandleInput();
}

TEST_F( PlayingStateInputHandlerTests, HandleInput_StartButtonWasPressed_ExecutesOpenPlayingMenuCommand )
{
   ON_CALL( *_inputReaderMock, WasButtonPressed( GameButton::Start ) ).WillByDefault( Return( true ) );
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::OpenPlayingMenu ) );

   _inputHandler->HandleInput();
}

TEST_F( PlayingStateInputHandlerTests, HandleInput_AButtonWasPressed_ExecutesJumpCommand )
{
   ON_CALL( *_inputReaderMock, WasButtonPressed( GameButton::A ) ).WillByDefault( Return( true ) );
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::Jump ) );

   _inputHandler->HandleInput();
}

TEST_F( PlayingStateInputHandlerTests, HandleInput_AButtonWasNotPressedAndIsDown_ExecutesExtendJumpCommand )
{
   ON_CALL( *_inputReaderMock, IsButtonDown( GameButton::A ) ).WillByDefault( Return( true ) );
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::ExtendJump ) );

   _inputHandler->HandleInput();
}

TEST_F( PlayingStateInputHandlerTests, HandleInput_BButtonWasPressed_ExecutesShootCommand )
{
   ON_CALL( *_inputReaderMock, WasButtonPressed( GameButton::B ) ).WillByDefault( Return( true ) );
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::Shoot ) );

   _inputHandler->HandleInput();
}

TEST_F( PlayingStateInputHandlerTests, HandleInput_LeftButtonIsDown_ExecutesPushAndPointPlayerCommands )
{
   shared_ptr<GameCommandArgs> pushArgs;
   shared_ptr<GameCommandArgs> pointArgs;
   ON_CALL( *_inputReaderMock, IsButtonDown( GameButton::Left ) ).WillByDefault( Return( true ) );

   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::PushPlayer, _ ) ).WillOnce( SaveArg<1>( &pushArgs ) );
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::PointPlayer, _ ) ).WillOnce( SaveArg<1>( &pointArgs ) );

   _inputHandler->HandleInput();

   EXPECT_EQ( static_pointer_cast<PushPlayerCommandArgs>( pushArgs )->Direction, Direction::Left );
   EXPECT_EQ( static_pointer_cast<PointPlayerCommandArgs>( pointArgs )->Direction, Direction::Left );
}

TEST_F( PlayingStateInputHandlerTests, HandleInput_UpAndLeftButtonsAreDown_ExecutesPushAndPointPlayerCommands )
{
   shared_ptr<GameCommandArgs> pushArgs;
   shared_ptr<GameCommandArgs> pointArgs;
   ON_CALL( *_inputReaderMock, IsButtonDown( GameButton::Up ) ).WillByDefault( Return( true ) );
   ON_CALL( *_inputReaderMock, IsButtonDown( GameButton::Left ) ).WillByDefault( Return( true ) );

   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::PushPlayer, _ ) ).WillOnce( SaveArg<1>( &pushArgs ) );
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::PointPlayer, _ ) ).WillOnce( SaveArg<1>( &pointArgs ) );

   _inputHandler->HandleInput();

   EXPECT_EQ( static_pointer_cast<PushPlayerCommandArgs>( pushArgs )->Direction, Direction::Left );
   EXPECT_EQ( static_pointer_cast<PointPlayerCommandArgs>( pointArgs )->Direction, Direction::UpLeft );
}

TEST_F( PlayingStateInputHandlerTests, HandleInput_UpAndRightButtonsAreDown_ExecutesPushPlayerRightCommand )
{
   shared_ptr<GameCommandArgs> pushArgs;
   shared_ptr<GameCommandArgs> pointArgs;
   ON_CALL( *_inputReaderMock, IsButtonDown( GameButton::Up ) ).WillByDefault( Return( true ) );
   ON_CALL( *_inputReaderMock, IsButtonDown( GameButton::Right ) ).WillByDefault( Return( true ) );

   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::PushPlayer, _ ) ).WillOnce( SaveArg<1>( &pushArgs ) );
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::PointPlayer, _ ) ).WillOnce( SaveArg<1>( &pointArgs ) );

   _inputHandler->HandleInput();

   EXPECT_EQ( static_pointer_cast<PushPlayerCommandArgs>( pushArgs )->Direction, Direction::Right );
   EXPECT_EQ( static_pointer_cast<PointPlayerCommandArgs>( pointArgs )->Direction, Direction::UpRight );
}

TEST_F( PlayingStateInputHandlerTests, HandleInput_RightButtonIsDown_ExecutesPushPlayerRightCommand )
{
   shared_ptr<GameCommandArgs> pushArgs;
   shared_ptr<GameCommandArgs> pointArgs;
   ON_CALL( *_inputReaderMock, IsButtonDown( GameButton::Right ) ).WillByDefault( Return( true ) );

   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::PushPlayer, _ ) ).WillOnce( SaveArg<1>( &pushArgs ) );
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::PointPlayer, _ ) ).WillOnce( SaveArg<1>( &pointArgs ) );

   _inputHandler->HandleInput();

   EXPECT_EQ( static_pointer_cast<PushPlayerCommandArgs>( pushArgs )->Direction, Direction::Right );
   EXPECT_EQ( static_pointer_cast<PointPlayerCommandArgs>( pointArgs )->Direction, Direction::Right );
}

TEST_F( PlayingStateInputHandlerTests, HandleInput_DownAndRightButtonsAreDown_ExecutesPushPlayerRightCommand )
{
   shared_ptr<GameCommandArgs> pushArgs;
   shared_ptr<GameCommandArgs> pointArgs;
   ON_CALL( *_inputReaderMock, IsButtonDown( GameButton::Down ) ).WillByDefault( Return( true ) );
   ON_CALL( *_inputReaderMock, IsButtonDown( GameButton::Right ) ).WillByDefault( Return( true ) );

   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::PushPlayer, _ ) ).WillOnce( SaveArg<1>( &pushArgs ) );
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::PointPlayer, _ ) ).WillOnce( SaveArg<1>( &pointArgs ) );

   _inputHandler->HandleInput();

   EXPECT_EQ( static_pointer_cast<PushPlayerCommandArgs>( pushArgs )->Direction, Direction::Right );
   EXPECT_EQ( static_pointer_cast<PointPlayerCommandArgs>( pointArgs )->Direction, Direction::DownRight );
}

TEST_F( PlayingStateInputHandlerTests, HandleInput_DownAndLeftButtonsAreDown_ExecutesPushPlayerLeftCommand )
{
   shared_ptr<GameCommandArgs> pushArgs;
   shared_ptr<GameCommandArgs> pointArgs;
   ON_CALL( *_inputReaderMock, IsButtonDown( GameButton::Down ) ).WillByDefault( Return( true ) );
   ON_CALL( *_inputReaderMock, IsButtonDown( GameButton::Left ) ).WillByDefault( Return( true ) );

   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::PushPlayer, _ ) ).WillOnce( SaveArg<1>( &pushArgs ) );
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::PointPlayer, _ ) ).WillOnce( SaveArg<1>( &pointArgs ) );

   _inputHandler->HandleInput();

   EXPECT_EQ( static_pointer_cast<PushPlayerCommandArgs>( pushArgs )->Direction, Direction::Left );
   EXPECT_EQ( static_pointer_cast<PointPlayerCommandArgs>( pointArgs )->Direction, Direction::DownLeft );
}

TEST_F( PlayingStateInputHandlerTests, HandleInput_NoButtonsAreDown_DoesNotExecuteAnyCommand )
{
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( _, _ ) ).Times( 0 );

   _inputHandler->HandleInput();
}
