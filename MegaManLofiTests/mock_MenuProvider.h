#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IMenuProvider.h>

class mock_MenuProvider : public MegaManLofi::IMenuProvider
{
public:
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::IMenu>, GetMenu, ( MegaManLofi::MenuType ), ( const, override ) );
};
