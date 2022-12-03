#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/GameEventAggregator.h>
#include <MegaManLofi/GameEvent.h>

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class ShutdownEventHandler
{
public:
   ShutdownEventHandler( const shared_ptr<IGameEventAggregator> eventAggregator )
   {
      eventAggregator->RegisterEventHandler( GameEvent::Shutdown, std::bind( &ShutdownEventHandler::HandleQuitEvent, this ) );
   }

   bool HasHandledEvent = false;

private:
   void HandleQuitEvent() { HasHandledEvent = true; }
};

class GameEventAggregatorTests : public Test
{
public:
   void SetUp() override
   {
      _eventAggregator.reset( new GameEventAggregator );
   }

protected:
   shared_ptr<GameEventAggregator> _eventAggregator;
};

TEST_F( GameEventAggregatorTests, RaiseEvent_HandlerHasBeenRegistered_HandlerIsNotified )
{
   ShutdownEventHandler handler( _eventAggregator );

   EXPECT_FALSE( handler.HasHandledEvent );

   _eventAggregator->RaiseEvent( GameEvent::Shutdown );

   EXPECT_TRUE( handler.HasHandledEvent );
}

TEST_F( GameEventAggregatorTests, RaiseEvent_MultipleHandlersHaveBeenRegistered_AllHandlersAreNotified )
{
   ShutdownEventHandler handler1( _eventAggregator );
   ShutdownEventHandler handler2( _eventAggregator );

   EXPECT_FALSE( handler1.HasHandledEvent );
   EXPECT_FALSE( handler2.HasHandledEvent );

   _eventAggregator->RaiseEvent( GameEvent::Shutdown );

   EXPECT_TRUE( handler1.HasHandledEvent );
   EXPECT_TRUE( handler2.HasHandledEvent );
}
