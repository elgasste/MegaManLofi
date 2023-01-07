#pragma once

#include <memory>

namespace MegaManLofi
{
   class IFrameRateProvider;
   class FrameActionRegistry;
   class Stage;
   class Entity;

   class EntityPhysics
   {
   public:
      EntityPhysics() { }
      EntityPhysics( const std::shared_ptr<IFrameRateProvider> frameRateProvider,
                     const std::shared_ptr<FrameActionRegistry> frameActionRegistry );

      virtual void AssignTo( const std::shared_ptr<Stage> stage );
      virtual void Tick();

   private:
      void ApplyGravity( const std::shared_ptr<Entity> entity );
      void ApplyFriction( const std::shared_ptr<Entity> entity );

   private:
      const std::shared_ptr<IFrameRateProvider> _frameRateProvider;
      const std::shared_ptr<FrameActionRegistry> _frameActionRegistry;

      std::shared_ptr<Stage> _stage;
   };
}
