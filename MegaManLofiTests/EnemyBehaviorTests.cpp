#include "gtest/gtest.h"

#include <MegaManLofi/EnemyBehavior.h>

#include "mock_FrameRateProvider.h"
#include "mock_PlayerInfoProvider.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class EnemyBehaviorTests : public Test
{
public:
   void SetUp() override
   {
      _frameRateProviderMock.reset( new NiceMock<mock_FrameRateProvider> );
      _playerInfoProviderMock.reset( new NiceMock<mock_PlayerInfoProvider> );

      _behavior.reset( new EnemyBehavior( _frameRateProviderMock, _playerInfoProviderMock ) );
   }

protected:
   shared_ptr<mock_FrameRateProvider> _frameRateProviderMock;
   shared_ptr<mock_PlayerInfoProvider> _playerInfoProviderMock;

   shared_ptr<EnemyBehavior> _behavior;
};

TEST_F( EnemyBehaviorTests, Tick_GetFrameSecondsCommand_GetsFrameSeconds )
{
   auto instruction = (mbc_instruction)( MBCGET_FRAMESECS << MBC_CMD_SHIFT | 5 << MBC_ARG0_SHIFT );
   _behavior->SetInstructions( vector<mbc_instruction> { instruction } );

   EXPECT_CALL( *_frameRateProviderMock, GetFrameSeconds() );

   _behavior->Tick();
}
