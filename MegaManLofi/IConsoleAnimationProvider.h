#pragma once

#include <memory>

#include "ConsoleAnimationType.h"

namespace MegaManLofi
{
   class IConsoleAnimation;

   class __declspec( novtable ) IConsoleAnimationProvider
   {
   public:
      virtual const std::shared_ptr<IConsoleAnimation> GetAnimation( ConsoleAnimationType type ) const = 0;
   };
}
