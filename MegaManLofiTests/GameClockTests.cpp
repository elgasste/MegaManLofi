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
   EXPECT_EQ( _clock->GetMinimumFrameRate(), -1 );
   EXPECT_FALSE( _clock->HasMinimumFrameRate() );
   EXPECT_EQ( _clock->GetLagFrameCount(), 0 );
   EXPECT_EQ( _clock->GetElapsedNanoseconds(), 0 );
   EXPECT_EQ( _clock->GetCurrentFrame(), 0 );
   EXPECT_EQ( _clock->GetAverageFrameRate(), 0 );
   EXPECT_EQ( _clock->GetFrameSeconds(), 0 );
}

TEST_F( GameClockTests, SetMinimumFrameRate_FrameRateIsZero_ThrowsException )
{
   string message;

   try
   {
      _clock->SetMinimumFrameRate( 0 );
   }
   catch ( invalid_argument e )
   {
      message = e.what();
   }

   EXPECT_EQ( message, "Minimum frame rate cannot be zero" );
}

TEST_F( GameClockTests, SetMinimumFrameRate_FrameRateIsPositive_SetsMinimumFrameRate )
{
   _clock->SetMinimumFrameRate( 60 );

   EXPECT_TRUE( _clock->HasMinimumFrameRate() );
   EXPECT_EQ( _clock->GetMinimumFrameRate(), 60 );
}

TEST_F( GameClockTests, SetMinimumFrameRate_FrameRateIsNegative_RemovesMinimumFrameRate )
{
   _clock->SetMinimumFrameRate( 20 );
   EXPECT_TRUE( _clock->HasMinimumFrameRate() );

   _clock->SetMinimumFrameRate( -20 );

   EXPECT_FALSE( _clock->HasMinimumFrameRate() );
   EXPECT_EQ( _clock->GetMinimumFrameRate(), -1 );
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

TEST_F( GameClockTests, GetFrameSeconds_DoesNotHaveMinimumFrameRateAlways_ReturnsTotalFrameSeconds )
{
   _clock->StartFrame();
   EXPECT_CALL( *_highResolutionClockMock, Now() ).WillOnce( Return( 1'000'000'000 ) ); // 1 second
   _clock->EndFrame();

   EXPECT_EQ( _clock->GetFrameSeconds(), 1. );
}

TEST_F( GameClockTests, GetFrameSeconds_FrameWasFasterThanMinimumFrameRate_ReturnsTotalFrameSeconds )
{
   _clock->SetMinimumFrameRate( 10 ); // 100,000,000 nanoseconds per frame

   _clock->StartFrame();
   EXPECT_CALL( *_highResolutionClockMock, Now() ).WillOnce( Return( 90'000'000 ) );
   _clock->EndFrame();

   EXPECT_EQ( _clock->GetFrameSeconds(), 0.09f );
   EXPECT_EQ( _clock->GetLagFrameCount(), 0 );
}

TEST_F( GameClockTests, GetFrameSeconds_FrameWasEqualToMinimumFrameRate_ReturnsTotalFrameSeconds )
{
   _clock->SetMinimumFrameRate( 10 ); // 100,000,000 nanoseconds per frame

   _clock->StartFrame();
   EXPECT_CALL( *_highResolutionClockMock, Now() ).WillOnce( Return( 100'000'000 ) );
   _clock->EndFrame();

   EXPECT_EQ( _clock->GetFrameSeconds(), 0.1f );
   EXPECT_EQ( _clock->GetLagFrameCount(), 0 );
}

TEST_F( GameClockTests, GetFrameSeconds_FrameWasSlowerThanMinimumFrameRate_ReturnsMinimumFrameSeconds )
{
   _clock->SetMinimumFrameRate( 10 ); // 100,000,000 nanoseconds per frame

   _clock->StartFrame();
   EXPECT_CALL( *_highResolutionClockMock, Now() ).WillOnce( Return( 110'000'000 ) );
   _clock->EndFrame();

   EXPECT_EQ( _clock->GetFrameSeconds(), 0.1f );
   EXPECT_EQ( _clock->GetLagFrameCount(), 1 );
}
