#include "gtest/gtest.h"

#include <memory>
#include <stdexcept>

#include <MegaManLofi/GameClock.h>

#include "mock_HighResolutionClock.h"
#include "mock_Sleeper.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class GameClockTests : public Test
{
public:
   void SetUp() override
   {
      _highResolutionClockMock.reset( new NiceMock<mock_HighResolutionClock> );
      _sleeperMock.reset( new NiceMock<mock_Sleeper> );
      _framesPerSecond = 100;

      _clock.reset( new GameClock( _highResolutionClockMock,
                                   _sleeperMock,
                                   _framesPerSecond ) );

      ON_CALL( *_highResolutionClockMock, Now() ).WillByDefault( Return( 0 ) );
   }

protected:
   shared_ptr<mock_HighResolutionClock> _highResolutionClockMock;
   shared_ptr<mock_Sleeper> _sleeperMock;
   unsigned int _framesPerSecond;

   shared_ptr<GameClock> _clock;
};

TEST_F( GameClockTests, Constructor_Always_InitializesAllValues )
{
   EXPECT_EQ( _clock->GetFramesPerSecond(), 100 );
   EXPECT_EQ( _clock->GetTotalFrameCount(), 0 );
   EXPECT_EQ( _clock->GetLagFrameCount(), 0 );
   EXPECT_EQ( _clock->GetSecondsPerFrame(), .01 );
}

TEST_F( GameClockTests, Constructor_FrameRateIsZero_ThrowsException )
{
   string message = "";
   try
   {
      auto clock = shared_ptr<GameClock>( new GameClock( _highResolutionClockMock, _sleeperMock, 0 ) );
   }
   catch ( invalid_argument e )
   {
      message = e.what();
   }

   EXPECT_EQ( message, "Frame rate cannot be zero" );
}

TEST_F( GameClockTests, StartFrame_Always_SetsFrameStartTime )
{
   EXPECT_CALL( *_highResolutionClockMock, Now() );

   _clock->StartFrame();
}

TEST_F( GameClockTests, WaitForNextFrame_Always_SetsFrameEndTime )
{
   EXPECT_CALL( *_highResolutionClockMock, Now() );

   _clock->StartFrame();

   EXPECT_CALL( *_highResolutionClockMock, Now() );

   _clock->WaitForNextFrame();
}

TEST_F( GameClockTests, WaitForNextFrame_Always_IncrementsTotalFrames )
{
   _clock->StartFrame();
   _clock->WaitForNextFrame();

   EXPECT_EQ( _clock->GetTotalFrameCount(), 1 );
}

TEST_F( GameClockTests, WaitForNextFrame_TimeIsLeftInFrame_SleepsForRemainingTime )
{
   _clock->StartFrame();

   ON_CALL( *_highResolutionClockMock, Now() ).WillByDefault( Return( 1'000'000ll ) );

   EXPECT_CALL( *_sleeperMock, Sleep( 9'000'000ll ) );

   _clock->WaitForNextFrame();
}

TEST_F( GameClockTests, WaitForNextFrame_FrameIsOverTime_IncrementsLagFrames )
{
   _clock->StartFrame();

   auto nanoSecondsPerFrame = 1'000'000'000ll / _framesPerSecond;

   ON_CALL( *_highResolutionClockMock, Now() ).WillByDefault( Return( nanoSecondsPerFrame + 1ll ) );

   EXPECT_CALL( *_sleeperMock, Sleep( _ ) ).Times( 0 );

   _clock->WaitForNextFrame();

   EXPECT_EQ( _clock->GetLagFrameCount(), 1 );
}
