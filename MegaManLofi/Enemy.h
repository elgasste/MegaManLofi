#pragma once

namespace MegaManLofi
{
   class Enemy
   {
   public:
      void SetObjectId( long long objectId ) { _objectId = objectId; }
      void SetX( long long x ) { _x = x; }
      void SetY( long long y ) { _y = y; }

   private:
      long long _objectId;
      long long _x;
      long long _y;
   };
}

