#include "gtest/gtest.h"

#include <MegaManLofi/Stage.h>
#include <MegaManLofi/StageDefs.h>

#include "mock_Arena.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

#include "mock_GameEventAggregator.h"

class StageTests : public Test
{
public:
   void SetUp() override
   {
      _stageDefs.reset( new StageDefs );
      _eventAggregatorMock.reset( new NiceMock<mock_GameEventAggregator> );
      _arenaMock1.reset( new NiceMock<mock_Arena> );
      _arenaMock2.reset( new NiceMock<mock_Arena> );

      _stageDefs->StartArenaId = 1;

      ON_CALL( *_arenaMock1, GetArenaId() ).WillByDefault( Return( 1 ) );
      ON_CALL( *_arenaMock2, GetArenaId() ).WillByDefault( Return( 2 ) );
   }

   void BuildStage()
   {
      _stage.reset( new Stage( _stageDefs, _eventAggregatorMock ) );
   }

protected:
   shared_ptr<StageDefs> _stageDefs;
   shared_ptr<mock_GameEventAggregator> _eventAggregatorMock;
   shared_ptr<mock_Arena> _arenaMock1;
   shared_ptr<mock_Arena> _arenaMock2;

   shared_ptr<Stage> _stage;
};

TEST_F( StageTests, Constructor_Always_SetsActiveArenaFromDefs )
{
   _stageDefs->StartArenaId = 2;
   BuildStage();
   _stage->AddArena( _arenaMock1 );
   _stage->AddArena( _arenaMock2 );

   EXPECT_EQ( _stage->GetActiveArena(), _arenaMock2 );
}

TEST_F( StageTests, Reset_Always_ClearsActiveArena )
{
   BuildStage();
   _stage->AddArena( _arenaMock1 );

   EXPECT_CALL( *_arenaMock1, Clear() );

   _stage->Reset();
}

TEST_F( StageTests, Reset_Always_ResetsActiveArenaFromDefs )
{
   BuildStage();
   _stage->AddArena( _arenaMock1 );
   _stage->AddArena( _arenaMock2 );
   _stage->SetActiveArena( 2 );

   EXPECT_EQ( _stage->GetActiveArena(), _arenaMock2 );

   _stage->Reset();

   EXPECT_EQ( _stage->GetActiveArena(), _arenaMock1 );
}

TEST_F( StageTests, GetArena_Always_ReturnsCorrectArena )
{
   BuildStage();
   _stage->AddArena( _arenaMock1 );
   _stage->AddArena( _arenaMock2 );

   EXPECT_EQ( _stage->GetArena( 1 ), _arenaMock1 );
   EXPECT_EQ( _stage->GetArena( 2 ), _arenaMock2 );
}

TEST_F( StageTests, GetArenaPortal_PortalNotFoundForDirection_ReturnsNoValue )
{
   BuildStage();

   auto portal = _stage->GetArenaPortal( Direction::Left, 1 );

   EXPECT_EQ( portal, nullptr );
}

TEST_F( StageTests, GetArenaPortal_PortalNotFoundForArenaId_ReturnsNoValue )
{
   _stageDefs->ArenaPortalMap[Direction::Left].push_back( shared_ptr<ArenaPortal>( new ArenaPortal( { 2, 1, 0, 0, 0 } ) ) ); // from 2 to 1
   BuildStage();

   auto portal = _stage->GetArenaPortal( Direction::Left, 1 );

   EXPECT_EQ( portal, nullptr );
}

TEST_F( StageTests, GetArenaPortal_PortalFound_ReturnsPortal )
{
   _stageDefs->ArenaPortalMap[Direction::Left].push_back( shared_ptr<ArenaPortal>( new ArenaPortal( { 1, 2, 0, 0, 0 } ) ) ); // from 1 to 2
   BuildStage();

   auto portal = _stage->GetArenaPortal( Direction::Left, 1 );

   EXPECT_EQ( portal->FromArenaId, 1 );
   EXPECT_EQ( portal->ToArenaId, 2 );
}

TEST_F( StageTests, SetActiveArena_ActiveArenaDoesNotChange_DoesNotRaiseActiveArenaChangedEvent )
{
   BuildStage();
   _stage->AddArena( _arenaMock1 );

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( _ ) ).Times( 0 );

   _stage->SetActiveArena( 1 );
}

TEST_F( StageTests, SetActiveArena_ActiveArenaChanges_ClearsOldActiveArena )
{
   BuildStage();
   _stage->AddArena( _arenaMock1 );
   _stage->AddArena( _arenaMock2 );
   EXPECT_EQ( _stage->GetActiveArena(), _arenaMock1 );

   EXPECT_CALL( *_arenaMock1, Clear() );

   _stage->SetActiveArena( 2 );
}

TEST_F( StageTests, SetActiveArena_ActiveArenaChanges_ChangesActiveArena )
{
   BuildStage();
   _stage->AddArena( _arenaMock1 );
   _stage->AddArena( _arenaMock2 );
   EXPECT_EQ( _stage->GetActiveArena(), _arenaMock1 );

   _stage->SetActiveArena( 2 );

   EXPECT_EQ( _stage->GetActiveArena(), _arenaMock2 );
}

TEST_F( StageTests, SetActiveArena_ActiveArenaChanges_RaisesActiveArenaChangedEvent )
{
   BuildStage();
   _stage->AddArena( _arenaMock1 );
   _stage->AddArena( _arenaMock2 );
   EXPECT_EQ( _stage->GetActiveArena(), _arenaMock1 );

   EXPECT_CALL( *_eventAggregatorMock, RaiseEvent( GameEvent::ActiveArenaChanged ) );

   _stage->SetActiveArena( 2 );
}
