#pragma once

#include <map>

#include "FrameAction.h"

namespace MegaManLofi
{
   class FrameActionRegistry
   {
   public:
      FrameActionRegistry();

      virtual void Clear();
      virtual void FlagAction( FrameAction action );
      virtual bool ActionFlagged( FrameAction action ) const;

   private:
      std::map<FrameAction, bool> _frameActions;
   };
}
