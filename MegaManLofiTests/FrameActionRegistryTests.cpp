#include "gtest/gtest.h"

#include <MegaManLofi/FrameActionRegistry.h>
#include <MegaManLofi/FrameAction.h>

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class FrameActionRegistryTests : public Test
{
public:
   void SetUp() override
   {
      _registry.reset( new FrameActionRegistry );
   }

protected:
   shared_ptr<FrameActionRegistry> _registry;
};

TEST_F( FrameActionRegistryTests, Constructor_Always_ClearsRegistry )
{
   for ( int i = 0; i < (int)FrameAction::FrameActionCount; i++ )
   {
      EXPECT_FALSE( _registry->ActionFlagged( (FrameAction)i ) );
   }
}

TEST_F( FrameActionRegistryTests, Clear_Always_ClearsRegistry )
{
   for ( int i = 0; i < (int)FrameAction::FrameActionCount; i++ )
   {
      _registry->FlagAction( (FrameAction)i );
      EXPECT_TRUE( _registry->ActionFlagged( (FrameAction)i ) );
   }

   _registry->Clear();

   for ( int i = 0; i < (int)FrameAction::FrameActionCount; i++ )
   {
      EXPECT_FALSE( _registry->ActionFlagged( (FrameAction)i ) );
   }
}

TEST_F( FrameActionRegistryTests, FlagAction_Always_FlagsAction )
{
   EXPECT_FALSE( _registry->ActionFlagged( FrameAction::PlayerJumping ) );

   _registry->FlagAction( FrameAction::PlayerJumping );

   EXPECT_TRUE( _registry->ActionFlagged( FrameAction::PlayerJumping ) );
}

TEST_F( FrameActionRegistryTests, ActionFlagged_ActionIsNotFlagged_ReturnsFalse )
{
   EXPECT_FALSE( _registry->ActionFlagged( FrameAction::PlayerJumping ) );
}

TEST_F( FrameActionRegistryTests, ActionFlagged_ActionIsFlagged_ReturnsTrue )
{
   _registry->FlagAction( FrameAction::PlayerJumping );

   EXPECT_TRUE( _registry->ActionFlagged( FrameAction::PlayerJumping ) );
}
