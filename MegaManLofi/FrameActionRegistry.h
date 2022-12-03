#pragma once

#include <map>

#include "IFrameActionRegistry.h"

namespace MegaManLofi
{
   class FrameActionRegistry : public IFrameActionRegistry
   {
   public:
      FrameActionRegistry();

      void Clear() override;
      void FlagAction( FrameAction action ) override;
      bool ActionFlagged( FrameAction action ) const override;

   private:
      std::map<FrameAction, bool> _frameActions;
   };
}
