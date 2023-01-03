#include "FrameActionRegistry.h"

using namespace std;
using namespace MegaManLofi;

FrameActionRegistry::FrameActionRegistry()
{
   Clear();
}

void FrameActionRegistry::Clear()
{
   for ( int i = 0; i < (int)FrameAction::FrameActionCount; i++ )
   {
      _frameActions[(FrameAction)i] = false;
   }
}

void FrameActionRegistry::FlagAction( FrameAction action )
{
   _frameActions[action] = true;
}

bool FrameActionRegistry::ActionFlagged( FrameAction action ) const
{
   return _frameActions.at( action );
}
