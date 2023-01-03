#include "PlayingMenuStateConsoleRenderer.h"
#include "ConsoleBuffer.h"
#include "ConsoleRenderDefs.h"
#include "ConsoleSpriteDefs.h"
#include "IConsoleSprite.h"
#include "IMenuProvider.h"
#include "IMenu.h"
#include "MenuType.h"

using namespace std;
using namespace MegaManLofi;

PlayingMenuStateConsoleRenderer::PlayingMenuStateConsoleRenderer( const shared_ptr<ConsoleBuffer> consoleBuffer,
                                                                  const shared_ptr<ConsoleRenderDefs> renderDefs,
                                                                  const shared_ptr<IMenuProvider> menuProvider ) :
   _consoleBuffer( consoleBuffer ),
   _renderDefs( renderDefs ),
   _menuProvider( menuProvider )
{
}

void PlayingMenuStateConsoleRenderer::Render()
{
   _consoleBuffer->SetDefaultForegroundColor( _renderDefs->PlayingMenuForegroundColor );
   _consoleBuffer->SetDefaultBackgroundColor( _renderDefs->PlayingMenuBackgroundColor );

   const auto& menu = _menuProvider->GetMenu( MenuType::Playing );
   int top = ( _renderDefs->ConsoleHeightChars / 2 ) - ( menu->GetOptionCount() / 2 );
   int leftOffset = 40;

   _consoleBuffer->Draw( leftOffset, top + menu->GetSelectedIndex() - 3, _renderDefs->PlayingMenuPlayerImage );

   for ( int i = 0; i < menu->GetOptionCount(); i++, top++ )
   {
      if ( menu->GetSelectedIndex() == i )
      {
         _consoleBuffer->Draw( leftOffset + 18, top, _renderDefs->SpriteDefs->MenuCaratSprite );
      }
      _consoleBuffer->Draw( leftOffset + 20, top, menu->GetOptionTitle( i ) );
   }

   _renderDefs->SpriteDefs->MenuCaratSprite->Tick();
}
