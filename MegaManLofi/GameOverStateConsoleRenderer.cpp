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
   _consoleBuffer->SetDefaultBackgroundColor( ConsoleColor::DarkMagenta );
   _consoleBuffer->SetDefaultForegroundColor( ConsoleColor::White );

   auto left = _renderConfig->ArenaViewportWidthChar / 2;
   auto top = _renderConfig->ArenaViewportHeightChar / 2;

   _consoleBuffer->Draw( left - 5, top - 1, "GAME OVER!" );
   _consoleBuffer->Draw( left - 15, top + 1, "(press any button to continue)" );
}
