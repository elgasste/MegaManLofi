#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <io.h>
#include <fcntl.h>

#include <iostream>

#include "ConsoleBuffer.h"
#include "HighResolutionClockWrapper.h"
#include "KeyboardWrapper.h"
#include "ThreadWrapper.h"
#include "RandomWrapper.h"
#include "GameEventAggregator.h"
#include "FrameActionRegistry.h"
#include "GameClock.h"
#include "UniqueNumberGenerator.h"
#include "GameDefsGenerator.h"
#include "GameDefs.h"
#include "ConsoleRenderDefs.h"
#include "KeyboardInputDefs.h"
#include "KeyboardInputReader.h"
#include "PlayerPhysics.h"
#include "ArenaPhysics.h"
#include "Player.h"
#include "Arena.h"
#include "EntityFactory.h"
#include "Game.h"
#include "PlayingMenu.h"
#include "MenuRepository.h"
#include "TitleStateInputHandler.h"
#include "PlayingStateInputHandler.h"
#include "PlayingMenuStateInputHandler.h"
#include "GameOverStateInputHandler.h"
#include "GameInputHandler.h"
#include "PlayerThwipOutConsoleAnimation.h"
#include "StageStartedConsoleAnimation.h"
#include "PlayerThwipInConsoleAnimation.h"
#include "PlayerExplodedConsoleAnimation.h"
#include "ConsoleAnimationRepository.h"
#include "EntityConsoleSpriteCopier.h"
#include "EntityConsoleSpriteRepository.h"
#include "DiagnosticsConsoleRenderer.h"
#include "TitleStateConsoleRenderer.h"
#include "PlayingStateConsoleRenderer.h"
#include "PlayingMenuStateConsoleRenderer.h"
#include "GameOverStateConsoleRenderer.h"
#include "GameRenderer.h"
#include "GameRunner.h"

using namespace std;
using namespace MegaManLofi;

// TODO: I suppose all the game objects should be loaded from disk at some point,
// but for now let's just do it all in here.
void LoadAndRun( const shared_ptr<IConsoleBuffer> consoleBuffer );

INT WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR lpCmdLine, _In_ INT nCmdShow )
{
   AllocConsole();
   wstring consoleTitle = L"Console Game";
   SetConsoleTitle( consoleTitle.c_str() );

   // you can't close this window unless I WANT you to
   EnableMenuItem( GetSystemMenu( GetConsoleWindow(), FALSE ), SC_CLOSE , MF_GRAYED );
   DrawMenuBar( GetConsoleWindow() );

   auto consoleHandleR = _open_osfhandle( (intptr_t)GetStdHandle( STD_INPUT_HANDLE ), _O_TEXT );
   auto fptr = _fdopen( consoleHandleR, "r" );
   *stdin = *fptr;
   setvbuf( stdin, NULL, _IONBF, 0 );

   auto consoleHandleW = _open_osfhandle( (intptr_t)GetStdHandle( STD_OUTPUT_HANDLE ), _O_TEXT );
   fptr = _fdopen( consoleHandleW, "w" );
   *stdout = *fptr;
   setvbuf( stdout, NULL, _IONBF, 0 );

   *stderr = *fptr;
   setvbuf( stderr, NULL, _IONBF, 0 );

   auto consoleBuffer = shared_ptr<ConsoleBuffer>( new ConsoleBuffer() );
   LoadAndRun( consoleBuffer );
}

void LoadAndRun( const shared_ptr<IConsoleBuffer> consoleBuffer )
{
   consoleBuffer->Draw( 2, 1, "Loading all the things..." );
   consoleBuffer->Flip();

   // wrappers
   auto highResolutionClock = make_shared<HighResolutionClockWrapper>();
   auto keyboard = make_shared<KeyboardWrapper>();
   auto thread = make_shared<ThreadWrapper>();
   auto random = make_shared<RandomWrapper>();

   // auxiliary objects
   auto eventAggregator = make_shared<GameEventAggregator>();
   auto frameActionRegistry = make_shared<FrameActionRegistry>();
   auto clock = shared_ptr<GameClock>( new GameClock( highResolutionClock ) );
   auto uniqueNumberGenerator = make_shared<UniqueNumberGenerator>();

   // game defs
   auto gameDefs = GameDefsGenerator::GenerateGameDefs( clock, uniqueNumberGenerator );
   if ( gameDefs->MinimumFrameRate > 0 )
   {
      clock->SetMinimumFrameRate( gameDefs->MinimumFrameRate );
   }
   auto consoleRenderDefs = static_pointer_cast<ConsoleRenderDefs>( gameDefs->RenderDefs );
   auto keyboardInputDefs = static_pointer_cast<KeyboardInputDefs>( gameDefs->InputDefs );

   // input
   auto keyboardInputReader = shared_ptr<KeyboardInputReader>( new KeyboardInputReader( keyboardInputDefs, keyboard ) );

   // utilities
   auto playerPhysics = shared_ptr<PlayerPhysics>( new PlayerPhysics( clock, frameActionRegistry, gameDefs->PlayerPhysicsDefs ) );
   auto arenaPhysics = shared_ptr<ArenaPhysics>( new ArenaPhysics( clock, eventAggregator, gameDefs->ArenaDefs ) );

   // game objects
   auto player = shared_ptr<Player>( new Player( gameDefs->PlayerDefs, frameActionRegistry, clock ) );
   auto arena = shared_ptr<Arena>( new Arena( gameDefs->ArenaDefs, eventAggregator ) );
   auto entityFactory = shared_ptr<EntityFactory>( new EntityFactory( gameDefs->EntityDefs, uniqueNumberGenerator ) );
   auto game = shared_ptr<Game>( new Game( eventAggregator, player, arena, playerPhysics, arenaPhysics, entityFactory ) );

   // menus
   auto playingMenu = shared_ptr<PlayingMenu>( new PlayingMenu( game ) );
   auto menuRepository = make_shared<MenuRepository>();
   menuRepository->AddMenu( MenuType::Playing, playingMenu );

   // input objects
   auto startupStateInputHandler = shared_ptr<TitleStateInputHandler>( new TitleStateInputHandler( keyboardInputReader, game ) );
   auto playingStateInputHandler = shared_ptr<PlayingStateInputHandler>( new PlayingStateInputHandler( keyboardInputReader, game ) );
   auto playingMenuStateInputHandler = shared_ptr<PlayingMenuStateInputHandler>( new PlayingMenuStateInputHandler( keyboardInputReader, game, menuRepository ) );
   auto gameOverStateInputHandler = shared_ptr<GameOverStateInputHandler>( new GameOverStateInputHandler( keyboardInputReader, game ) );
   auto inputHandler = shared_ptr<GameInputHandler>( new GameInputHandler( keyboardInputReader, game, eventAggregator ) );
   inputHandler->AddInputHandlerForGameState( GameState::Title, startupStateInputHandler );
   inputHandler->AddInputHandlerForGameState( GameState::Playing, playingStateInputHandler );
   inputHandler->AddInputHandlerForGameState( GameState::PlayingMenu, playingMenuStateInputHandler );
   inputHandler->AddInputHandlerForGameState( GameState::GameOver, gameOverStateInputHandler );

   // animations
   auto playerThwipOutAnimation = shared_ptr<PlayerThwipOutConsoleAnimation>( new PlayerThwipOutConsoleAnimation( consoleBuffer, consoleRenderDefs, clock ) );
   auto stageStartedAnimation = shared_ptr<StageStartedConsoleAnimation>( new StageStartedConsoleAnimation( consoleBuffer, clock, consoleRenderDefs ) );
   auto playerThwipInAnimation = shared_ptr<PlayerThwipInConsoleAnimation>( new PlayerThwipInConsoleAnimation( consoleBuffer, consoleRenderDefs, clock ) );
   auto playerExplodedAnimation = shared_ptr<PlayerExplodedConsoleAnimation>( new PlayerExplodedConsoleAnimation( consoleBuffer, clock, consoleRenderDefs ) );
   auto animationRepository = make_shared<ConsoleAnimationRepository>();
   animationRepository->AddAnimation( ConsoleAnimationType::PlayerThwipOut, playerThwipOutAnimation );
   animationRepository->AddAnimation( ConsoleAnimationType::StageStarted, stageStartedAnimation );
   animationRepository->AddAnimation( ConsoleAnimationType::PlayerThwipIn, playerThwipInAnimation );
   animationRepository->AddAnimation( ConsoleAnimationType::PlayerExploded, playerExplodedAnimation );

   // rendering utilities
   auto spriteCopier = shared_ptr<IEntityConsoleSpriteCopier>( new EntityConsoleSpriteCopier );
   auto spriteRepository = shared_ptr<EntityConsoleSpriteRepository>( new EntityConsoleSpriteRepository( eventAggregator, arena, spriteCopier, consoleRenderDefs->SpriteDefs ) );

   // renderers objects
   auto diagnosticsRenderer = shared_ptr<DiagnosticsConsoleRenderer>( new DiagnosticsConsoleRenderer( consoleBuffer, clock, consoleRenderDefs, game, spriteRepository ) );
   auto titleStateConsoleRenderer = shared_ptr<TitleStateConsoleRenderer>( new TitleStateConsoleRenderer( consoleBuffer, random, clock, eventAggregator, consoleRenderDefs, keyboardInputDefs, animationRepository ) );
   auto playingStateConsoleRenderer = shared_ptr<PlayingStateConsoleRenderer>( new PlayingStateConsoleRenderer( consoleBuffer, consoleRenderDefs, game, game, game, eventAggregator, clock, animationRepository, spriteRepository ) );
   auto playingMenuStateConsoleRenderer = shared_ptr<PlayingMenuStateConsoleRenderer>( new PlayingMenuStateConsoleRenderer( consoleBuffer, consoleRenderDefs, menuRepository ) );
   auto gameOverStateConsoleRenderer = shared_ptr<GameOverStateConsoleRenderer>( new GameOverStateConsoleRenderer( consoleBuffer, consoleRenderDefs ) );
   auto renderer = shared_ptr<GameRenderer>( new GameRenderer( consoleRenderDefs, consoleBuffer, game, diagnosticsRenderer, eventAggregator ) );
   renderer->AddRendererForGameState( GameState::Title, titleStateConsoleRenderer );
   renderer->AddRendererForGameState( GameState::Playing, playingStateConsoleRenderer );
   renderer->AddRendererForGameState( GameState::PlayingMenu, playingMenuStateConsoleRenderer );
   renderer->AddRendererForGameState( GameState::GameOver, gameOverStateConsoleRenderer );

   // game loop
   auto runner = shared_ptr<GameRunner>( new GameRunner( eventAggregator, clock, inputHandler, renderer, frameActionRegistry, game, thread ) );

   runner->Run();
}
