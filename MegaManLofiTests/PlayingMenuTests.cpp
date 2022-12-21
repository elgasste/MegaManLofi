#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/PlayingMenu.h>
#include <MegaManLofi/GameCommand.h>

#include "mock_GameCommandExecutor.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class PlayingMenuTests : public Test
{
public:
   void SetUp() override
   {
      _commandExecutorMock.reset( new NiceMock<mock_GameCommandExecutor> );

      _menu.reset( new PlayingMenu( _commandExecutorMock ) );
   }

protected:
   shared_ptr<mock_GameCommandExecutor> _commandExecutorMock;

   shared_ptr<PlayingMenu> _menu;
};

TEST_F( PlayingMenuTests, Constructor_Always_InitializesSelectedIndex )
{
   EXPECT_EQ( _menu->GetSelectedIndex(), 0 );
}

TEST_F( PlayingMenuTests, IncrementSelectedIndex_WithinRange_IncrementsSelectedIndex )
{
   _menu->IncrementSelectedIndex();

   EXPECT_EQ( _menu->GetSelectedIndex(), 1 );
}

TEST_F( PlayingMenuTests, IncrementSelectedIndex_OutOfRange_LoopsSelectedIndexBackToZero )
{
   _menu->IncrementSelectedIndex();
   _menu->IncrementSelectedIndex();
   _menu->IncrementSelectedIndex();

   EXPECT_EQ( _menu->GetSelectedIndex(), 0 );
}

TEST_F( PlayingMenuTests, DecrementSelectedIndex_WithinRange_DecrementsSelectedIndex )
{
   _menu->DecrementSelectedIndex();

   EXPECT_EQ( _menu->GetSelectedIndex(), 2 );
}

TEST_F( PlayingMenuTests, DecrementSelectedIndex_OutOfRange_LoopsSelectedIndexToEndOfList )
{
   _menu->DecrementSelectedIndex();

   EXPECT_EQ( _menu->GetSelectedIndex(), 2 );
}

TEST_F( PlayingMenuTests, ExecuteSelectedIndex_IndexIsZero_ExecutesClosePlayingMenuCommand )
{
   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::ClosePlayingMenu ) );

   _menu->ExecuteSelectedIndex();
}

TEST_F( PlayingMenuTests, ExecuteSelectedIndex_IndexIsOne_ExecutesExitToTitleCommand )
{
   _menu->IncrementSelectedIndex();

   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::ExitToTitle ) );

   _menu->ExecuteSelectedIndex();
}

TEST_F( PlayingMenuTests, ExecuteSelectedIndex_IndexIsZero_ExecutesQuitCommand )
{
   _menu->IncrementSelectedIndex();
   _menu->IncrementSelectedIndex();

   EXPECT_CALL( *_commandExecutorMock, ExecuteCommand( GameCommand::Quit ) );

   _menu->ExecuteSelectedIndex();
}
