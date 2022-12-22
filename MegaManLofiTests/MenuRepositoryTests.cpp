#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/MenuRepository.h>

#include "mock_Menu.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class MenuRepositoryTests : public Test { };

TEST_F( MenuRepositoryTests, AddMenu_Always_AddsMenuToMap )
{
   MenuRepository repository;

   auto animationMock = shared_ptr<mock_Menu>( new NiceMock<mock_Menu> );
   repository.AddMenu( MenuType::Playing, animationMock );

   EXPECT_EQ( repository.GetMenu( MenuType::Playing ), animationMock );
}

TEST_F( MenuRepositoryTests, GetMenu_Always_ReturnsCorrectMenu )
{
   MenuRepository repository;

   auto menuMock1 = shared_ptr<mock_Menu>( new NiceMock<mock_Menu> );
   auto menuMock2 = shared_ptr<mock_Menu>( new NiceMock<mock_Menu> );
   repository.AddMenu( MenuType::Playing, menuMock1 );
   repository.AddMenu( MenuType::Title, menuMock2 );

   EXPECT_EQ( repository.GetMenu( MenuType::Playing ), menuMock1 );
   EXPECT_EQ( repository.GetMenu( MenuType::Title ), menuMock2 );
}
