#include "gtest/gtest.h"

#include <MegaManLofi/Stage.h>
#include <MegaManLofi/StageDefs.h>

#include "mock_Arena.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class StageTests : public Test
{
public:
   void SetUp() override
   {
      _stageDefs.reset( new StageDefs );
      _arenaMock1.reset( new NiceMock<mock_Arena> );
      _arenaMock2.reset( new NiceMock<mock_Arena> );

      _stageDefs->StartArenaId = 1;

      ON_CALL( *_arenaMock1, GetArenaId() ).WillByDefault( Return( 1 ) );
      ON_CALL( *_arenaMock2, GetArenaId() ).WillByDefault( Return( 2 ) );
   }

   void BuildStage()
   {
      _stage.reset( new Stage( _stageDefs ) );
   }

protected:
   shared_ptr<StageDefs> _stageDefs;
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
