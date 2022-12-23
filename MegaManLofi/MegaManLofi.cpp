#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <io.h>
#include <fcntl.h>

#include <iostream>

#include "GameConfig.h"
#include "ConsoleRenderConfig.h"
#include "KeyboardInputConfig.h"
#include "PlayerConfig.h"
#include "ArenaConfig.h"
#include "PlayerPhysicsConfig.h"
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

using namespace std;
using namespace MegaManLofi;

// TODO: I suppose these configs should be loaded from files at some point,
// but at the very least they should all have default values, and those could
// probably be set in some initializer instead of in here.
shared_ptr<ConsoleRenderConfig> BuildConsoleRenderConfig( const shared_ptr<IFrameRateProvider> frameRateProvider );
shared_ptr<KeyboardInputConfig> BuildKeyboardInputConfig();
shared_ptr<PlayerConfig> BuildPlayerConfig();
shared_ptr<ArenaConfig> BuildArenaConfig();
shared_ptr<PlayerPhysicsConfig> BuildPlayerPhysicsConfig();
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
   config->InputConfig = BuildKeyboardInputConfig();
   config->PlayerConfig = BuildPlayerConfig();
   config->ArenaConfig = BuildArenaConfig();
   config->PlayerPhysicsConfig = BuildPlayerPhysicsConfig();
   auto consoleRenderConfig = static_pointer_cast<ConsoleRenderConfig>( config->RenderConfig );
   auto keyboardInputConfig = static_pointer_cast<KeyboardInputConfig>( config->InputConfig );

   // input
   auto keyboardInputReader = shared_ptr<KeyboardInputReader>( new KeyboardInputReader( keyboardInputConfig, keyboard ) );

   // utilities
   auto playerPhysics = shared_ptr<PlayerPhysics>( new PlayerPhysics( clock, frameActionRegistry, config->PlayerPhysicsConfig ) );
   auto arenaPhysics = shared_ptr<ArenaPhysics>( new ArenaPhysics( clock, frameActionRegistry, eventAggregator ) );

   // game objects
   auto player = shared_ptr<Player>( new Player( config->PlayerConfig, frameActionRegistry, clock ) );
   auto arena = shared_ptr<Arena>( new Arena( config->ArenaConfig ) );
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
   auto titleStateConsoleRenderer = shared_ptr<TitleStateConsoleRenderer>( new TitleStateConsoleRenderer( consoleBuffer, random, clock, eventAggregator, consoleRenderConfig, keyboardInputConfig, animationRepository ) );
   auto playingStateConsoleRenderer = shared_ptr<PlayingStateConsoleRenderer>( new PlayingStateConsoleRenderer( consoleBuffer, consoleRenderConfig, game, player, arena, eventAggregator, clock, animationRepository ) );
   auto playingMenuStateConsoleRenderer = shared_ptr<PlayingMenuStateConsoleRenderer>( new PlayingMenuStateConsoleRenderer( consoleBuffer, consoleRenderConfig, menuRepository ) );
   auto gameOverStateConsoleRenderer = shared_ptr<GameOverStateConsoleRenderer>( new GameOverStateConsoleRenderer( consoleBuffer, consoleRenderConfig, keyboardInputConfig ) );
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

shared_ptr<KeyboardInputConfig> BuildKeyboardInputConfig()
{
   auto inputConfig = make_shared<KeyboardInputConfig>();

   // key code bindings
   inputConfig->KeyMap[KeyCode::Left] = GameButton::Left;
   inputConfig->KeyMap[KeyCode::Up] = GameButton::Up;
   inputConfig->KeyMap[KeyCode::Right] = GameButton::Right;
   inputConfig->KeyMap[KeyCode::Down] = GameButton::Down;

   inputConfig->KeyMap[KeyCode::Return] = GameButton::Start;
   inputConfig->KeyMap[KeyCode::RShiftKey] = GameButton::Select;

   inputConfig->KeyMap[KeyCode::A] = GameButton::A;
   inputConfig->KeyMap[KeyCode::B] = GameButton::B;
   inputConfig->KeyMap[KeyCode::X] = GameButton::X;
   inputConfig->KeyMap[KeyCode::Y] = GameButton::Y;

   inputConfig->KeyMap[KeyCode::NumPad1] = GameButton::L1;
   inputConfig->KeyMap[KeyCode::NumPad2] = GameButton::L2;
   inputConfig->KeyMap[KeyCode::NumPad3] = GameButton::R1;
   inputConfig->KeyMap[KeyCode::NumPad4] = GameButton::R2;

   inputConfig->KeyMap[KeyCode::F12] = GameButton::Diagnostics;

   // key names
   inputConfig->KeyNames[KeyCode::Left] = "Left Arrow";
   inputConfig->KeyNames[KeyCode::Up] = "Up Arrow";
   inputConfig->KeyNames[KeyCode::Right] = "Right Arrow";
   inputConfig->KeyNames[KeyCode::Down] = "Down Arrow";
   inputConfig->KeyNames[KeyCode::Return] = "Enter";
   inputConfig->KeyNames[KeyCode::LShiftKey] = "Left Shift";
   inputConfig->KeyNames[KeyCode::RShiftKey] = "Right Shift";
   inputConfig->KeyNames[KeyCode::Space] = "Space Bar";
   inputConfig->KeyNames[KeyCode::Tab] = "Tab";
   inputConfig->KeyNames[KeyCode::A] = "A";
   inputConfig->KeyNames[KeyCode::B] = "B";
   inputConfig->KeyNames[KeyCode::X] = "X";
   inputConfig->KeyNames[KeyCode::Y] = "Y";
   inputConfig->KeyNames[KeyCode::NumPad1] = "1";
   inputConfig->KeyNames[KeyCode::NumPad2] = "2";
   inputConfig->KeyNames[KeyCode::NumPad3] = "3";
   inputConfig->KeyNames[KeyCode::NumPad4] = "4";
   inputConfig->KeyNames[KeyCode::F12] = "F12";

   // button names
   inputConfig->ButtonNames[GameButton::Left] = "Left";
   inputConfig->ButtonNames[GameButton::Up] = "Up";
   inputConfig->ButtonNames[GameButton::Right] = "Right";
   inputConfig->ButtonNames[GameButton::Down] = "Down";
   inputConfig->ButtonNames[GameButton::Start] = "Start";
   inputConfig->ButtonNames[GameButton::Select] = "Select";
   inputConfig->ButtonNames[GameButton::A] = "A";
   inputConfig->ButtonNames[GameButton::B] = "B";
   inputConfig->ButtonNames[GameButton::X] = "X";
   inputConfig->ButtonNames[GameButton::Y] = "Y";
   inputConfig->ButtonNames[GameButton::L1] = "L1";
   inputConfig->ButtonNames[GameButton::L2] = "L2";
   inputConfig->ButtonNames[GameButton::R1] = "R1";
   inputConfig->ButtonNames[GameButton::R2] = "R2";
   inputConfig->ButtonNames[GameButton::Diagnostics] = "Toggle Diagnostics";

   return inputConfig;
}

shared_ptr<PlayerConfig> BuildPlayerConfig()
{
   auto playerConfig = make_shared<PlayerConfig>();

   // one character is 38 x 78 units, and our player sprites are 4 x 3 characters,
   // so this hit box should match the player's sprite size
   playerConfig->DefaultHitBox = { 0, 0, 38 * 4, 78 * 3 };

   playerConfig->DefaultVelocityX = 0;
   playerConfig->DefaultVelocityY = 0;

   playerConfig->DefaultLives = 3;
   playerConfig->DefaultDirection = Direction::Right;

   return playerConfig;
}

// TODO: move all of this into ArenaGenerator
shared_ptr<ArenaConfig> BuildArenaConfig()
{
   auto arenaConfig = make_shared<ArenaConfig>();

   // this results in a 4560 x 2340 unit viewport, which translates super well to a 120 x 30 character console
   arenaConfig->DefaultTileWidth = 38;
   arenaConfig->DefaultTileHeight = 78;

   arenaConfig->DefaultHorizontalTiles = 360;
   arenaConfig->DefaultVerticalTiles = 60;

   arenaConfig->DefaultTiles = ArenaTileGenerator::GenerateArenaTiles();

   arenaConfig->DefaultPlayerPosition = { arenaConfig->DefaultTileWidth * 8, arenaConfig->DefaultTileHeight * 6 };

   return arenaConfig;
}

shared_ptr<PlayerPhysicsConfig> BuildPlayerPhysicsConfig()
{
   auto playerPhysicsConfig = make_shared<PlayerPhysicsConfig>();

   playerPhysicsConfig->MaxPushVelocity = 1'200;
   playerPhysicsConfig->MaxGravityVelocity = 4'000;

   playerPhysicsConfig->PushAccelerationPerSecond = 8'500;
   playerPhysicsConfig->FrictionDecelerationPerSecond = 10'000;
   playerPhysicsConfig->JumpAccelerationPerSecond = 2'000;
   playerPhysicsConfig->GravityAccelerationPerSecond = 10'000;

   playerPhysicsConfig->MaxJumpExtensionSeconds = 0.25;

   return playerPhysicsConfig;
}

shared_ptr<GameConfig> BuildGameConfig()
{
   auto config = make_shared<GameConfig>();

   config->FramesPerSecond = 60;

   return config;
}
