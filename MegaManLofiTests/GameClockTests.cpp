#include "gtest/gtest.h"

#include <memory>
#include <stdexcept>

#include <MegaManLofi/GameClock.h>

#include "mock_HighResolutionClock.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class GameClockTests : public Test
{
public:
   void SetUp() override
   {
      _highResolutionClockMock.reset( new NiceMock<mock_HighResolutionClock> );

      _clock.reset( new GameClock( _highResolutionClockMock ) );

      ON_CALL( *_highResolutionClockMock, Now() ).WillByDefault( Return( 0 ) );
   }

protected:
   shared_ptr<mock_HighResolutionClock> _highResolutionClockMock;

   shared_ptr<GameClock> _clock;
};

TEST_F( GameClockTests, Constructor_Always_InitializesAllValues )
{
   EXPECT_EQ( _clock->GetElapsedNanoseconds(), 0 );
   EXPECT_EQ( _clock->GetCurrentFrame(), 0 );
   EXPECT_EQ( _clock->GetAverageFrameRate(), 0 );
   EXPECT_EQ( _clock->GetFrameSeconds(), 0 );
}

TEST_F( GameClockTests, StartFrame_Always_SetsFrameStartTime )
{
   EXPECT_CALL( *_highResolutionClockMock, Now() );

   _clock->StartFrame();
}

TEST_F( GameClockTests, EndFrame_Always_UpdatesProperties )
{
   _clock->StartFrame();

   EXPECT_CALL( *_highResolutionClockMock, Now() ).WillOnce( Return( 500'000'000 ) ); // 0.5 seconds

   _clock->EndFrame();

   EXPECT_EQ( _clock->GetElapsedNanoseconds(), 500'000'000 );
   EXPECT_EQ( _clock->GetCurrentFrame(), 1 );
   EXPECT_EQ( _clock->GetAverageFrameRate(), 2 );
   EXPECT_EQ( _clock->GetFrameSeconds(), .5 );
}
