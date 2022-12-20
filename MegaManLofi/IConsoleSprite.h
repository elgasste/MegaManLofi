#pragma once

#include "ConsoleImage.h"

namespace MegaManLofi
{
   class __declspec( novtable ) IConsoleSprite
   {
   public:
      virtual void AddImage( ConsoleImage image ) = 0;
      virtual void Tick( int framesPerSecond ) = 0;
      virtual void Reset() = 0;

      virtual short GetWidth() const = 0;
      virtual short GetHeight() const = 0;
      virtual double GetTotalTraversalSeconds() const = 0;
      virtual const ConsoleImage& GetCurrentImage() const = 0;;
   };
}
