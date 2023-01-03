#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/EntityConsoleSpriteCopier.h>

class mock_EntityConsoleSpriteCopier : public MegaManLofi::EntityConsoleSpriteCopier
{
public:
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::EntityConsoleSprite>, MakeCopy, ( const std::shared_ptr<MegaManLofi::EntityConsoleSprite> ), ( const, override ) );
};
