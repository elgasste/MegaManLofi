#include "PlayerExplodedConsoleAnimation.h"
#include "IConsoleBuffer.h"
#include "IFrameRateProvider.h"
#include "ConsoleRenderConfig.h"

using namespace std;
using namespace MegaManLofi;

PlayerExplodedConsoleAnimation::PlayerExplodedConsoleAnimation( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                                const shared_ptr<IFrameRateProvider> frameRateProvider,
                                                                const shared_ptr<ConsoleRenderConfig> renderConfig ) :
   _consoleBuffer( consoleBuffer ),
   _frameRateProvider( frameRateProvider ),
   _renderConfig( renderConfig ),
   _isRunning( false )
{
}

void PlayerExplodedConsoleAnimation::Start( Coordinate<short> startPositionChars, Coordinate<short> endPositionChars )
{
   _isRunning = true;
}

void PlayerExplodedConsoleAnimation::Draw()
{

}

void PlayerExplodedConsoleAnimation::Tick()
{

}
