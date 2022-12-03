#pragma once

#include <string>

#include "IScreenBuffer.h"

namespace MegaManLofi
{
   enum class ConsoleColor;
   struct ConsoleSprite;

   class __declspec( novtable ) IConsoleBuffer : public IScreenBuffer
   {
   public:
      virtual void LoadRenderConfig( std::shared_ptr<IGameRenderConfig> config ) override = 0;
      virtual void CleanUp() override = 0;

      virtual void SetDefaultForegroundColor( ConsoleColor color ) = 0;
      virtual void SetDefaultBackgroundColor( ConsoleColor color ) = 0;
      virtual void Clear() override = 0;

      virtual void Draw( int left, int top, char buffer ) = 0;
      virtual void Draw( int left, int top, char buffer, ConsoleColor foregroundColor ) = 0;
      virtual void Draw( int left, int top, char buffer, ConsoleColor foregroundColor, ConsoleColor backgroundColor ) = 0;
      virtual void Draw( int left, int top, const std::string& buffer ) = 0;
      virtual void Draw( int left, int top, const std::string& buffer, ConsoleColor foregroundColor ) = 0;
      virtual void Draw( int left, int top, const std::string& buffer, ConsoleColor foregroundColor, ConsoleColor backgroundColor ) = 0;
      virtual void Draw( int left, int top, const ConsoleSprite& sprite ) = 0;

      virtual void Flip() override = 0;
   };
}
