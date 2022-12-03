#pragma once

namespace MegaManLofi
{
   class __declspec( novtable ) IThread
   {
   public:
      virtual bool SetThisThreadToHighestPriority() const = 0;
   };
}
