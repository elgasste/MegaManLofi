#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IMenu.h>

class mock_Menu : public MegaManLofi::IMenu
{
public:
   MOCK_METHOD( int, GetOptionCount, ( ), ( const, override ) );
   MOCK_METHOD( const std::string&, GetOptionTitle, ( int ), ( const, override ) );
   MOCK_METHOD( int, GetSelectedIndex, ( ), ( const, override ) );
   MOCK_METHOD( void, IncrementSelectedIndex, ( ), ( override ) );
   MOCK_METHOD( void, DecrementSelectedIndex, ( ), ( override ) );
   MOCK_METHOD( void, ExecuteSelectedIndex, ( ), ( override ) );
};
