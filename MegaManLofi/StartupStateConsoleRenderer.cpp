#include <string>
#include <format>

#include "StartupStateConsoleRenderer.h"
#include "IConsoleBuffer.h"
#include "ConsoleRenderConfig.h"
#include "KeyboardInputConfig.h"
#include "ConsoleColor.h"

using namespace std;
using namespace MegaManLofi;

StartupStateConsoleRenderer::StartupStateConsoleRenderer( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                          const shared_ptr<ConsoleRenderConfig> renderConfig,
                                                          const shared_ptr<KeyboardInputConfig> inputConfig ) :
   _consoleBuffer( consoleBuffer ),
   _renderConfig( renderConfig ),
   _inputConfig( inputConfig )
{
}

void StartupStateConsoleRenderer::Render()
{
   _consoleBuffer->SetDefaultForegroundColor( _renderConfig->TitleScreenForegroundColor );
   _consoleBuffer->SetDefaultBackgroundColor( _renderConfig->TitleScreenBackgroundColor );

   _consoleBuffer->Draw( _renderConfig->TitleTextX, _renderConfig->TitleTextY, _renderConfig->TitleTextSprite );
   _consoleBuffer->Draw( _renderConfig->TitleSubTextX, _renderConfig->TitleSubTextY, _renderConfig->TitleSubTextSprite );
   _consoleBuffer->Draw( _renderConfig->TitlePlayerX, _renderConfig->TitlePlayerY, _renderConfig->TitlePlayerSprite );
   _consoleBuffer->Draw( _renderConfig->TitleBuildingX, _renderConfig->TitleBuildingY, _renderConfig->TitleBuildingSprite );

   DrawKeyBindings();
}

void StartupStateConsoleRenderer::DrawKeyBindings() const
{
   auto leftOfMiddleX = _renderConfig->TitleKeyBindingsMiddleX - 2;
   auto top = _renderConfig->TitleKeyBindingsY;

   for ( auto const& [keyCode, mappedButton] : _inputConfig->KeyMap )
   {
      string keyString( format( "{0} Key", _inputConfig->KeyNames.at(keyCode) ) );
      string buttonString( format( "{0} Button", _inputConfig->ButtonNames.at(mappedButton) ) );

      _consoleBuffer->Draw( leftOfMiddleX - (int)keyString.length() - 2, top, format( "{0} -> {1}", keyString, buttonString ), _renderConfig->TitleKeyBindingsForegroundColor );

      top++;
   }
}
