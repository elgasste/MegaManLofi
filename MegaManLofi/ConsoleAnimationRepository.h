#pragma once

#include <map>

#include "IConsoleAnimationProvider.h"

namespace MegaManLofi
{
   class ConsoleAnimationRepository : public IConsoleAnimationProvider
   {
   public:
      void AddAnimation( ConsoleAnimationType type, std::shared_ptr<IConsoleAnimation> animation );

      const std::shared_ptr<IConsoleAnimation> GetAnimation( ConsoleAnimationType type ) const;

   private:
      std::map<ConsoleAnimationType, std::shared_ptr<IConsoleAnimation>> _animationMap;
   };
}
