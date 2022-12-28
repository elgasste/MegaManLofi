#pragma once

#include <memory>
#include <string>

#include "IScreenBuffer.h"
#include "ConsoleImage.h"

namespace MegaManLofi
{
   class IConsoleSprite;
   class IEntityConsoleSprite;

   class __declspec( novtable ) IConsoleBuffer : public IScreenBuffer
   {
   public:
      virtual void LoadRenderDefs( std::shared_ptr<IGameRenderDefs> renderDefs ) override = 0;
      virtual void CleanUp() override = 0;

      virtual void SetDefaultForegroundColor( ConsoleColor color ) = 0;
      virtual void SetDefaultBackgroundColor( ConsoleColor color ) = 0;
      virtual void Clear() override = 0;

      virtual void Draw( short left, short top, char buffer ) = 0;
      virtual void Draw( short left, short top, char buffer, ConsoleColor foregroundColor ) = 0;
      virtual void Draw( short left, short top, char buffer, ConsoleColor foregroundColor, ConsoleColor backgroundColor ) = 0;
      virtual void Draw( short left, short top, const std::string& buffer ) = 0;
      virtual void Draw( short left, short top, const std::string& buffer, ConsoleColor foregroundColor ) = 0;
      virtual void Draw( short left, short top, const std::string& buffer, ConsoleColor foregroundColor, ConsoleColor backgroundColor ) = 0;
      virtual void Draw( short left, short top, const ConsoleImage& image ) = 0;
      virtual void Draw( short left, short top, const std::shared_ptr<IConsoleSprite> sprite ) = 0;
      virtual void Draw( short left, short top, const std::shared_ptr<IEntityConsoleSprite> sprite ) = 0;

      virtual void Flip() override = 0;
   };
}
