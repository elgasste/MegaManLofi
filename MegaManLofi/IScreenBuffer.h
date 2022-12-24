#pragma once

#include <memory>

namespace MegaManLofi
{
   class IGameRenderDefs;

   class __declspec( novtable ) IScreenBuffer
   {
   public:
      virtual void LoadRenderDefs( const std::shared_ptr<IGameRenderDefs> renderDefs ) = 0;
      virtual void CleanUp() = 0;

      virtual void Clear() = 0;
      virtual void Flip() = 0;
   };
}
