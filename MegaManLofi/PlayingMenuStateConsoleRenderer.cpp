#include "PlayingMenuStateConsoleRenderer.h"
#include "IConsoleBuffer.h"
#include "ConsoleRenderConfig.h"
#include "IConsoleSprite.h"
#include "IMenuProvider.h"
#include "IMenu.h"
#include "MenuType.h"

using namespace std;
using namespace MegaManLofi;

PlayingMenuStateConsoleRenderer::PlayingMenuStateConsoleRenderer( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                                  const shared_ptr<ConsoleRenderConfig> renderConfig,
                                                                  const shared_ptr<IMenuProvider> menuProvider ) :
   _consoleBuffer( consoleBuffer ),
   _renderConfig( renderConfig ),
   _menuProvider( menuProvider )
{
}

void PlayingMenuStateConsoleRenderer::Render()
{
   _consoleBuffer->SetDefaultForegroundColor( _renderConfig->PlayingMenuForegroundColor );
   _consoleBuffer->SetDefaultBackgroundColor( _renderConfig->PlayingMenuBackgroundColor );

   const auto& menu = _menuProvider->GetMenu( MenuType::Playing );
   int top = 10;

   for ( int i = 0; i < menu->GetOptionCount(); i++, top++ )
   {
      if ( menu->GetSelectedIndex() == i )
      {
         _consoleBuffer->Draw( 18, top, _renderConfig->MenuCaratSprite );
      }
      _consoleBuffer->Draw( 20, top, menu->GetOptionTitle( i ) );
   }

   _renderConfig->MenuCaratSprite->Tick();
}
