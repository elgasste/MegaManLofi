#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/PlayingMenu.h>

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class PlayingMenuTests : public Test
{
public:
   void SetUp() override
   {
      _menu.reset( new PlayingMenu );
   }

protected:
   shared_ptr<PlayingMenu> _menu;
};

TEST_F( PlayingMenuTests, Constructor_Always_InitializesSelectedIndex )
{
   EXPECT_EQ( _menu->GetSelectedIndex(), 0 );
}
