#pragma once

#include <gmock/gmock.h>

#include <MegaManLofi/IThread.h>

class mock_Thread : public MegaManLofi::IThread
{
public:
   MOCK_METHOD( bool, SetThisThreadToHighestPriority, ( ), ( const, override ) );
};
