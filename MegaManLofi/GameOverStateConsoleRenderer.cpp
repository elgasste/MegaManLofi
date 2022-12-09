#include "GameOverStateConsoleRenderer.h"
#include "IConsoleBuffer.h"
#include "ConsoleRenderConfig.h"
#include "KeyboardInputConfig.h"
#include "ConsoleColor.h"

using namespace std;
using namespace MegaManLofi;

GameOverStateConsoleRenderer::GameOverStateConsoleRenderer( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                            const shared_ptr<ConsoleRenderConfig> renderConfig,
                                                            const shared_ptr<KeyboardInputConfig> inputConfig ) :
   _consoleBuffer( consoleBuffer ),
   _renderConfig( renderConfig ),
   _inputConfig( inputConfig )
{
}

void GameOverStateConsoleRenderer::Render()
{
   _consoleBuffer->SetDefaultBackgroundColor( _renderConfig->GameOverBackgroundColor );

   auto left = ( _renderConfig->ConsoleWidth / 2 ) - ( _renderConfig->GameOverSprite.Width / 2 );
   auto top = ( _renderConfig->ConsoleHeight / 2 ) - ( _renderConfig->GameOverSprite.Height / 2 );

   _consoleBuffer->Draw( left, top, _renderConfig->GameOverSprite );
}
