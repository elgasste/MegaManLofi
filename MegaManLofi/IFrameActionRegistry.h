#pragma once

namespace MegaManLofi
{
   enum class FrameAction;

   class __declspec( novtable ) IFrameActionRegistry
   {
   public:
      virtual void Clear() = 0;
      virtual void FlagAction( FrameAction action ) = 0;
      virtual bool ActionFlagged( FrameAction action ) const = 0;
   };
}
