#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IGameRunner
   {
   public:
      virtual void Run() = 0;
   };
}
