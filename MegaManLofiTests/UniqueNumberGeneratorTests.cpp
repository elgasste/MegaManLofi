#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/UniqueNumberGenerator.h>

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class UniqueNumberGeneratorTests : public Test
{
public:
   void SetUp() override
   {
      _generator.reset( new UniqueNumberGenerator );
   }

protected:
   shared_ptr<UniqueNumberGenerator> _generator;
};

TEST_F( UniqueNumberGeneratorTests, Constructor_Always_InitializesNextNumberCorrectly )
{
   EXPECT_EQ( _generator->GetNext(), 0 );
}

TEST_F( UniqueNumberGeneratorTests, GetNext_Always_GetsNextNumberIncrementally )
{
   EXPECT_EQ( _generator->GetNext(), 0 );
   EXPECT_EQ( _generator->GetNext(), 1 );
   EXPECT_EQ( _generator->GetNext(), 2 );
   EXPECT_EQ( _generator->GetNext(), 3 );
   EXPECT_EQ( _generator->GetNext(), 4 );
   EXPECT_EQ( _generator->GetNext(), 5 );
}
