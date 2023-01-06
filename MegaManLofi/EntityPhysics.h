#pragma once

#include <memory>

namespace MegaManLofi
{
   class Stage;

   class EntityPhysics
   {
   public:
      EntityPhysics();

      virtual void AssignTo( const std::shared_ptr<Stage> stage );

   private:
      std::shared_ptr<Stage> _stage;
   };
}
