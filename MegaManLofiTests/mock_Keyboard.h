#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IKeyboard.h>

class mock_Keyboard : public MegaManLofi::IKeyboard
{
public:
   MOCK_METHOD( bool, IsKeyDown, ( MegaManLofi::KeyCode ), ( const, override ) );
};
