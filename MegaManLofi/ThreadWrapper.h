#pragma once

#include "IThread.h"

namespace MegaManLofi
{
   class ThreadWrapper : public IThread
   {
   public:
      bool SetThisThreadToHighestPriority() const override;
   };
}
