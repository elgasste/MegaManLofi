#include "PlayingMenuStateConsoleRenderer.h"
#include "IConsoleBuffer.h"
#include "ConsoleRenderConfig.h"

using namespace std;
using namespace MegaManLofi;

PlayingMenuStateConsoleRenderer::PlayingMenuStateConsoleRenderer( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                                  const shared_ptr<ConsoleRenderConfig> renderConfig ) :
   _consoleBuffer( consoleBuffer ),
   _renderConfig( renderConfig )
{
}

void PlayingMenuStateConsoleRenderer::Render()
{
   // TODO
}
