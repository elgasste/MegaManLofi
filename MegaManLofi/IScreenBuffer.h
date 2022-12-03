#pragma once

#include <memory>

namespace MegaManLofi
{
   class IGameRenderConfig;

   class __declspec( novtable ) IScreenBuffer
   {
   public:
      virtual void LoadRenderConfig( const std::shared_ptr<IGameRenderConfig> config ) = 0;
      virtual void CleanUp() = 0;

      virtual void Clear() = 0;
      virtual void Flip() = 0;
   };
}
