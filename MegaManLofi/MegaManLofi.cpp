#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <io.h>
#include <fcntl.h>

#include <iostream>

#include "GameConfig.h"
#include "ConsoleRenderConfig.h"
#include "KeyCode.h"
#include "GameButton.h"
#include "HighResolutionClockWrapper.h"
#include "SleeperWrapper.h"
#include "KeyboardWrapper.h"
#include "ThreadWrapper.h"
#include "RandomWrapper.h"
#include "GameEventAggregator.h"
#include "GameClock.h"
#include "KeyboardInputReader.h"
#include "FrameActionRegistry.h"
#include "Player.h"
#include "Arena.h"
#include "PlayerPhysics.h"
#include "ArenaPhysics.h"
#include "Game.h"
#include "MenuType.h"
#include "PlayingMenu.h"
#include "MenuRepository.h"
#include "PlayerThwipOutConsoleAnimation.h"
#include "StageStartedConsoleAnimation.h"
#include "PlayerThwipInConsoleAnimation.h"
#include "PlayerExplodedConsoleAnimation.h"
#include "ConsoleAnimationRepository.h"
#include "DiagnosticsConsoleRenderer.h"
#include "TitleStateInputHandler.h"
#include "PlayingStateInputHandler.h"
#include "PlayingMenuStateInputHandler.h"
#include "GameOverStateInputHandler.h"
#include "GameInputHandler.h"
#include "ConsoleBuffer.h"
#include "TitleStateConsoleRenderer.h"
#include "PlayingStateConsoleRenderer.h"
#include "PlayingMenuStateConsoleRenderer.h"
#include "GameOverStateConsoleRenderer.h"
#include "GameRenderer.h"
#include "GameRunner.h"
#include "GameState.h"
#include "Direction.h"
#include "ArenaTile.h"
#include "IConsoleSprite.h"
#include "PlayerSpriteGenerator.h"
#include "ArenaTileGenerator.h"
#include "ArenaSpriteGenerator.h"
#include "TitleSpriteGenerator.h"
#include "MenuSpriteGenerator.h"
#include "KeyboardInputDefs.h"
#include "KeyboardInputDefsGenerator.h"
#include "PlayerDefsGenerator.h"
#include "ArenaDefsGenerator.h"
#include "PlayerPhysicsDefsGenerator.h"

using namespace std;
using namespace MegaManLofi;

// TODO: I suppose these configs should be loaded from files at some point,
// but at the very least they should all have default values, and those could
// probably be set in some initializer instead of in here.
shared_ptr<ConsoleRenderConfig> BuildConsoleRenderConfig( const shared_ptr<IFrameRateProvider> frameRateProvider );
shared_ptr<GameConfig> BuildGameConfig();
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

   // main config
   auto config = BuildGameConfig();

   // wrappers
   auto highResolutionClock = make_shared<HighResolutionClockWrapper>();
   auto sleeper = make_shared<SleeperWrapper>();
   auto keyboard = make_shared<KeyboardWrapper>();
   auto thread = make_shared<ThreadWrapper>();
   auto random = make_shared<RandomWrapper>();

   // auxiliary objects
   auto eventAggregator = make_shared<GameEventAggregator>();
   auto frameActionRegistry = make_shared<FrameActionRegistry>();
   auto clock = shared_ptr<GameClock>( new GameClock( highResolutionClock, sleeper, config->FramesPerSecond ) );

   // sub-configs
   config->RenderConfig = BuildConsoleRenderConfig( clock );
   config->InputDefs = KeyboardInputDefsGenerator::GenerateKeyboardInputDefs();
   config->PlayerDefs = PlayerDefsGenerator::GeneratePlayerDefs();
   config->ArenaDefs = ArenaDefsGenerator::GenerateArenaDefs();
   config->PlayerPhysicsDefs = PlayerPhysicsDefsGenerator::GeneratePlayerPhysicsDefs();
   auto consoleRenderConfig = static_pointer_cast<ConsoleRenderConfig>( config->RenderConfig );
   auto keyboardInputDefs = static_pointer_cast<KeyboardInputDefs>( config->InputDefs );

   // input
   auto keyboardInputReader = shared_ptr<KeyboardInputReader>( new KeyboardInputReader( keyboardInputDefs, keyboard ) );

   // utilities
   auto playerPhysics = shared_ptr<PlayerPhysics>( new PlayerPhysics( clock, frameActionRegistry, config->PlayerPhysicsDefs ) );
   auto arenaPhysics = shared_ptr<ArenaPhysics>( new ArenaPhysics( clock, frameActionRegistry, eventAggregator ) );

   // game objects
   auto player = shared_ptr<Player>( new Player( config->PlayerDefs, frameActionRegistry, clock ) );
   auto arena = shared_ptr<Arena>( new Arena( config->ArenaDefs ) );
   auto game = shared_ptr<Game>( new Game( eventAggregator, player, arena, playerPhysics, arenaPhysics ) );

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
   auto playerThwipOutAnimation = shared_ptr<PlayerThwipOutConsoleAnimation>( new PlayerThwipOutConsoleAnimation( consoleBuffer, consoleRenderConfig, clock ) );
   auto stageStartedAnimation = shared_ptr<StageStartedConsoleAnimation>( new StageStartedConsoleAnimation( consoleBuffer, clock, consoleRenderConfig ) );
   auto playerThwipInAnimation = shared_ptr<PlayerThwipInConsoleAnimation>( new PlayerThwipInConsoleAnimation( consoleBuffer, consoleRenderConfig, clock ) );
   auto playerExplodedAnimation = shared_ptr<PlayerExplodedConsoleAnimation>( new PlayerExplodedConsoleAnimation( consoleBuffer, clock, consoleRenderConfig ) );
   auto animationRepository = make_shared<ConsoleAnimationRepository>();
   animationRepository->AddAnimation( ConsoleAnimationType::PlayerThwipOut, playerThwipOutAnimation );
   animationRepository->AddAnimation( ConsoleAnimationType::StageStarted, stageStartedAnimation );
   animationRepository->AddAnimation( ConsoleAnimationType::PlayerThwipIn, playerThwipInAnimation );
   animationRepository->AddAnimation( ConsoleAnimationType::PlayerExploded, playerExplodedAnimation );

   // rendering objects
   auto diagnosticsRenderer = shared_ptr<DiagnosticsConsoleRenderer>( new DiagnosticsConsoleRenderer( consoleBuffer, clock, consoleRenderConfig ) );
   auto titleStateConsoleRenderer = shared_ptr<TitleStateConsoleRenderer>( new TitleStateConsoleRenderer( consoleBuffer, random, clock, eventAggregator, consoleRenderConfig, keyboardInputDefs, animationRepository ) );
   auto playingStateConsoleRenderer = shared_ptr<PlayingStateConsoleRenderer>( new PlayingStateConsoleRenderer( consoleBuffer, consoleRenderConfig, game, player, arena, eventAggregator, clock, animationRepository ) );
   auto playingMenuStateConsoleRenderer = shared_ptr<PlayingMenuStateConsoleRenderer>( new PlayingMenuStateConsoleRenderer( consoleBuffer, consoleRenderConfig, menuRepository ) );
   auto gameOverStateConsoleRenderer = shared_ptr<GameOverStateConsoleRenderer>( new GameOverStateConsoleRenderer( consoleBuffer, consoleRenderConfig ) );
   auto renderer = shared_ptr<GameRenderer>( new GameRenderer( consoleRenderConfig, consoleBuffer, game, diagnosticsRenderer, eventAggregator ) );
   renderer->AddRendererForGameState( GameState::Title, titleStateConsoleRenderer );
   renderer->AddRendererForGameState( GameState::Playing, playingStateConsoleRenderer );
   renderer->AddRendererForGameState( GameState::PlayingMenu, playingMenuStateConsoleRenderer );
   renderer->AddRendererForGameState( GameState::GameOver, gameOverStateConsoleRenderer );

   // game loop
   auto runner = shared_ptr<GameRunner>( new GameRunner( eventAggregator, clock, inputHandler, renderer, frameActionRegistry, game, thread ) );

   runner->Run();
}

shared_ptr<ConsoleRenderConfig> BuildConsoleRenderConfig( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto renderConfig = make_shared<ConsoleRenderConfig>();

   renderConfig->ConsoleWidthChars = 120;
   renderConfig->ConsoleHeightChars = 30;

   renderConfig->ArenaViewportLeftChars = 0;
   renderConfig->ArenaViewportTopChars = 0;

   renderConfig->ArenaCharWidth = 38;
   renderConfig->ArenaCharHeight = 78;

   renderConfig->ArenaViewportWidthChars = 120;
   renderConfig->ArenaViewportHeightChars = 30;

   renderConfig->ArenaStatusBarLeftChars = renderConfig->ArenaViewportLeftChars;
   renderConfig->ArenaStatusBarTopChars = renderConfig->ArenaViewportTopChars;
   renderConfig->ArenaStatusBarWidthChars = 20;

   renderConfig->PitfallAnimationSeconds = 2;

   // player explosion should swap between two sprites in 1/4 second increments
   renderConfig->PlayerExplosionAnimationSeconds = 3;
   renderConfig->PlayerExplosionParticleVelocity = 1'000;
   renderConfig->PlayerExplosionParticleSprite = PlayerSpriteGenerator::GenerateExplosionParticleSprite( frameRateProvider );

   renderConfig->DefaultForegroundColor = ConsoleColor::Grey;
   renderConfig->DefaultBackgroundColor = ConsoleColor::Black;

   renderConfig->TitleScreenForegroundColor = ConsoleColor::White;
   renderConfig->TitleScreenBackgroundColor = ConsoleColor::Black;

   renderConfig->ArenaForegroundColor = ConsoleColor::White;
   renderConfig->ArenaBackgroundColor = ConsoleColor::Black;

   renderConfig->PlayingMenuForegroundColor = ConsoleColor::White;
   renderConfig->PlayingMenuBackgroundColor = ConsoleColor::DarkBlue;

   renderConfig->GameOverBackgroundColor = ConsoleColor::DarkMagenta;

   renderConfig->TitleKeyBindingsForegroundColor = ConsoleColor::DarkGrey;

   renderConfig->TitleTextImage = TitleSpriteGenerator::GenerateTitleTextImage();
   renderConfig->TitleSubTextImage = TitleSpriteGenerator::GenerateTitleSubTextImage();
   renderConfig->TitlePlayerImage = TitleSpriteGenerator::GeneratePlayerImage();
   renderConfig->TitleBuildingImage = TitleSpriteGenerator::GenerateBuildingImage();
   renderConfig->TitleStartMessageImage = TitleSpriteGenerator::GenerateStartMessageImage();
   renderConfig->TitleStarImage = TitleSpriteGenerator::GenerateStarImage();

   renderConfig->PlayerThwipSprite = PlayerSpriteGenerator::GenerateThwipSprite( frameRateProvider );
   renderConfig->PlayerThwipInTransitionSprite = PlayerSpriteGenerator::GenerateThwipInTransitionSprite( frameRateProvider );
   renderConfig->PlayerThwipOutTransitionSprite = PlayerSpriteGenerator::GenerateThwipOutTransitionSprite( frameRateProvider );
   renderConfig->PlayerThwipVelocity = 5'000;
   renderConfig->PlayerPostThwipDelaySeconds = 1;

   renderConfig->TitleTextLeftChars = 6;
   renderConfig->TitleTextTopChars = 1;
   renderConfig->TitleSubTextLeftChars = renderConfig->TitleTextLeftChars + renderConfig->TitleTextImage.Width;
   renderConfig->TitleSubTextTopChars = renderConfig->TitleTextTopChars;
   renderConfig->TitleBuildingLeftChars = renderConfig->ConsoleWidthChars - renderConfig->TitleBuildingImage.Width - 1;
   renderConfig->TitleBuildingTopChars = renderConfig->ConsoleHeightChars - renderConfig->TitleBuildingImage.Height - 1;
   renderConfig->TitlePlayerLeftChars = renderConfig->TitleBuildingLeftChars + 6;
   renderConfig->TitlePlayerTopChars = renderConfig->TitleBuildingTopChars - renderConfig->TitlePlayerImage.Height;
   renderConfig->TitleStartMessageLeftChars = 55;
   renderConfig->TitleStartMessageTopChars = 15;
   renderConfig->TitleKeyBindingsMiddleXChars = 25;
   renderConfig->TitleKeyBindingsTopChars = renderConfig->TitleTextTopChars + renderConfig->TitleTextImage.Height + 3;
   renderConfig->TitleStarCount = 20;
   renderConfig->MinTitleStarVelocity = 200;
   renderConfig->MaxTitleStarVelocity = 2'000;

   renderConfig->GetReadySprite = ArenaSpriteGenerator::GenerateGetReadySprite( frameRateProvider );
   renderConfig->GetReadyAnimationSeconds = 2;

   renderConfig->PauseOverlayImage = ArenaSpriteGenerator::GeneratePauseOverlayImage();
   renderConfig->GameOverImage = ArenaSpriteGenerator::GenerateGameOverImage();

   renderConfig->MenuCaratSprite = MenuSpriteGenerator::GenerateMenuCaratSprite( frameRateProvider );
   renderConfig->PlayingMenuPlayerImage = MenuSpriteGenerator::GeneratePlayerImage();

   renderConfig->PlayerStandingSpriteMap = PlayerSpriteGenerator::GenerateStandingSpriteMap( frameRateProvider );
   renderConfig->PlayerWalkingSpriteMap = PlayerSpriteGenerator::GenerateWalkingSpriteMap( frameRateProvider );
   renderConfig->PlayerFallingSpriteMap = PlayerSpriteGenerator::GenerateFallingSpriteMap( frameRateProvider );

   // TODO: move this stuff into the generator
   // ground that is impassable in all directions
   renderConfig->ArenaImageMap[0].Width = 1;
   renderConfig->ArenaImageMap[0].Height = 1;
   renderConfig->ArenaImageMap[0].Pixels.push_back( { 'X', true, ConsoleColor::Yellow, ConsoleColor::Black } );

   // ground that is only impassable downward
   renderConfig->ArenaImageMap[1].Width = 1;
   renderConfig->ArenaImageMap[1].Height = 1;
   renderConfig->ArenaImageMap[1].Pixels.push_back( { '-', true, ConsoleColor::Yellow, ConsoleColor::Black } );

   // spike that is only impassable upward
   renderConfig->ArenaImageMap[2].Width = 1;
   renderConfig->ArenaImageMap[2].Height = 1;
   renderConfig->ArenaImageMap[2].Pixels.push_back( { '+', true, ConsoleColor::Red, ConsoleColor::Black } );

   renderConfig->ArenaTiles = ArenaSpriteGenerator::GenerateArenaTiles();

   return renderConfig;
}

shared_ptr<GameConfig> BuildGameConfig()
{
   auto config = make_shared<GameConfig>();

   config->FramesPerSecond = 60;

   return config;
}
