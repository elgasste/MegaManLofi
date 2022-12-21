#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/ConsoleAnimationRepository.h>

#include "mock_ConsoleAnimation.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class ConsoleAnimationRepositoryTests : public Test { };

TEST_F( ConsoleAnimationRepositoryTests, AddAnimation_Always_AddsAnimationToMap )
{
   ConsoleAnimationRepository repository;

   auto animationMock = shared_ptr<mock_ConsoleAnimation>( new NiceMock<mock_ConsoleAnimation> );
   repository.AddAnimation( ConsoleAnimationType::StageStarted, animationMock );

   EXPECT_EQ( repository.GetAnimation( ConsoleAnimationType::StageStarted ), animationMock );
}

TEST_F( ConsoleAnimationRepositoryTests, GetAnimation_Always_ReturnsCorrectAnimation )
{
   ConsoleAnimationRepository repository;

   auto animationMock1 = shared_ptr<mock_ConsoleAnimation>( new NiceMock<mock_ConsoleAnimation> );
   auto animationMock2 = shared_ptr<mock_ConsoleAnimation>( new NiceMock<mock_ConsoleAnimation> );
   repository.AddAnimation( ConsoleAnimationType::StageStarted, animationMock1 );
   repository.AddAnimation( ConsoleAnimationType::PlayerExploded, animationMock2 );

   EXPECT_EQ( repository.GetAnimation( ConsoleAnimationType::StageStarted ), animationMock1 );
   EXPECT_EQ( repository.GetAnimation( ConsoleAnimationType::PlayerExploded ), animationMock2 );
}
