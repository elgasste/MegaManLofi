#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IEntityConsoleSpriteCopier.h>

class mock_EntityConsoleSpriteCopier : public MegaManLofi::IEntityConsoleSpriteCopier
{
public:
   MOCK_METHOD( const std::shared_ptr<MegaManLofi::IEntityConsoleSprite>, MakeCopy, ( const std::shared_ptr<MegaManLofi::IEntityConsoleSprite> ), ( const, override ) );
};
