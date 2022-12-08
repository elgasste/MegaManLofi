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
#include "GameEventAggregator.h"
#include "GameClock.h"
#include "KeyboardInputReader.h"
#include "FrameActionRegistry.h"
#include "Player.h"
#include "Arena.h"
#include "PlayerPhysics.h"
#include "ArenaPhysics.h"
#include "Game.h"
#include "DiagnosticsConsoleRenderer.h"
#include "StartupStateInputHandler.h"
#include "PlayingStateInputHandler.h"
#include "GameOverStateInputHandler.h"
#include "GameInputHandler.h"
#include "ConsoleBuffer.h"
#include "StartupStateConsoleRenderer.h"
#include "PlayingStateConsoleRenderer.h"
#include "GameOverStateConsoleRenderer.h"
#include "GameRenderer.h"
#include "GameRunner.h"
#include "GameState.h"
#include "ConsoleColor.h"
#include "Direction.h"
#include "ArenaTile.h"
#include "PlayerSpriteGenerator.h"
#include "ArenaTileGenerator.h"
#include "ArenaConsoleSpriteGenerator.h"
#include "ConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

// TODO: I suppose these configs should be loaded from files at some point,
// but at the very least they should all have default values, and those could
// probably be set in some initializer instead of in here.
shared_ptr<ConsoleRenderConfig> BuildConsoleRenderConfig();
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

   // configs
   auto config = BuildGameConfig();
   auto consoleRenderConfig = static_pointer_cast<ConsoleRenderConfig>( config->RenderConfig );
   auto keyboardInputConfig = static_pointer_cast<KeyboardInputConfig>( config->InputConfig );

   // wrappers
   auto highResolutionClock = shared_ptr<HighResolutionClockWrapper>( new HighResolutionClockWrapper() );
   auto sleeper = shared_ptr<SleeperWrapper>( new SleeperWrapper() );
   auto keyboard = shared_ptr<IKeyboard>( new KeyboardWrapper() );
   auto thread = shared_ptr<IThread>( new ThreadWrapper() );

   // auxiliary objects
   auto eventAggregator = shared_ptr<GameEventAggregator>( new GameEventAggregator() );
   auto frameActionRegistry = make_shared<FrameActionRegistry>();
   auto clock = shared_ptr<GameClock>( new GameClock( highResolutionClock, sleeper, config->FramesPerSecond ) );
   auto keyboardInputReader = shared_ptr<KeyboardInputReader>( new KeyboardInputReader( keyboardInputConfig, keyboard ) );

   // utilities
   auto playerPhysics = shared_ptr<PlayerPhysics>( new PlayerPhysics( clock, frameActionRegistry, config->PlayerPhysicsConfig ) );
   auto arenaPhysics = shared_ptr<ArenaPhysics>( new ArenaPhysics( clock, frameActionRegistry, eventAggregator ) );

   // game objects
   auto player = shared_ptr<Player>( new Player( config->PlayerConfig, frameActionRegistry, clock ) );
   auto arena = shared_ptr<Arena>( new Arena( config->ArenaConfig ) );
   auto game = shared_ptr<Game>( new Game( eventAggregator, player, arena, playerPhysics, arenaPhysics ) );

   // input objects
   auto startupStateInputHandler = shared_ptr<StartupStateInputHandler>( new StartupStateInputHandler( keyboardInputReader, game ) );
   auto playingStateInputHandler = shared_ptr<PlayingStateInputHandler>( new PlayingStateInputHandler( keyboardInputReader, game ) );
   auto gameOverStateInputHandler = shared_ptr<GameOverStateInputHandler>( new GameOverStateInputHandler( keyboardInputReader, game ) );
   auto inputHandler = shared_ptr<GameInputHandler>( new GameInputHandler( keyboardInputReader, game, eventAggregator ) );
   inputHandler->AddInputHandlerForGameState( GameState::Startup, startupStateInputHandler );
   inputHandler->AddInputHandlerForGameState( GameState::Playing, playingStateInputHandler );
   inputHandler->AddInputHandlerForGameState( GameState::GameOver, gameOverStateInputHandler );

   // rendering objects
   auto diagnosticsRenderer = shared_ptr<DiagnosticsConsoleRenderer>( new DiagnosticsConsoleRenderer( consoleBuffer, clock, consoleRenderConfig ) );
   auto startupStateConsoleRenderer = shared_ptr<StartupStateConsoleRenderer>( new StartupStateConsoleRenderer( consoleBuffer, consoleRenderConfig, keyboardInputConfig ) );
   auto playingStateConsoleRenderer = shared_ptr<PlayingStateConsoleRenderer>( new PlayingStateConsoleRenderer( consoleBuffer, consoleRenderConfig, player, arena, eventAggregator, clock ) );
   auto gameOverStateConsoleRenderer = shared_ptr<GameOverStateConsoleRenderer>( new GameOverStateConsoleRenderer( consoleBuffer, consoleRenderConfig, keyboardInputConfig ) );
   auto renderer = shared_ptr<GameRenderer>( new GameRenderer( consoleRenderConfig, consoleBuffer, game, diagnosticsRenderer, eventAggregator ) );
   renderer->AddRendererForGameState( GameState::Startup, startupStateConsoleRenderer );
   renderer->AddRendererForGameState( GameState::Playing, playingStateConsoleRenderer );
   renderer->AddRendererForGameState( GameState::GameOver, gameOverStateConsoleRenderer );

   // game loop
   auto runner = shared_ptr<GameRunner>( new GameRunner( eventAggregator, clock, inputHandler, renderer, frameActionRegistry, game, thread ) );

   runner->Run();
}

shared_ptr<ConsoleRenderConfig> BuildConsoleRenderConfig()
{
   auto renderConfig = make_shared<ConsoleRenderConfig>();

   renderConfig->ConsoleWidth = 120;
   renderConfig->ConsoleHeight = 30;

   renderConfig->ArenaViewportX = 0;
   renderConfig->ArenaViewportY = 0;

   renderConfig->ArenaCharWidth = 38;
   renderConfig->ArenaCharHeight = 78;

   renderConfig->ArenaViewportWidthChar = 120;
   renderConfig->ArenaViewportHeightChar = 30;

   // "GET READY!" message should blink for 2 seconds
   renderConfig->GameStartSingleBlinkSeconds = .25;
   renderConfig->GameStartBlinkCount = 8;

   renderConfig->PitfallAnimationSeconds = 2;

   renderConfig->DefaultForegroundColor = ConsoleColor::Grey;
   renderConfig->DefaultBackgroundColor = ConsoleColor::Black;

   renderConfig->PlayerStaticSpriteMap = PlayerSpriteGenerator::GenerateStaticSpriteMap();
   renderConfig->PlayerMovingSpriteMap = PlayerSpriteGenerator::GenerateMovingSpriteMap();

   // ground that is impassable in all directions
   renderConfig->ArenaSpriteMap[0].Width = 1;
   renderConfig->ArenaSpriteMap[0].Height = 1;
   renderConfig->ArenaSpriteMap[0].Pixels.push_back( { 'X', ConsoleColor::Yellow } );

   // ground that is only impassable downward
   renderConfig->ArenaSpriteMap[1].Width = 1;
   renderConfig->ArenaSpriteMap[1].Height = 1;
   renderConfig->ArenaSpriteMap[1].Pixels.push_back( { '-', ConsoleColor::Yellow } );

   // spike that is only impassable upward
   renderConfig->ArenaSpriteMap[2].Width = 1;
   renderConfig->ArenaSpriteMap[2].Height = 1;
   renderConfig->ArenaSpriteMap[2].Pixels.push_back( { 'M', ConsoleColor::Red } );

   renderConfig->ArenaSprites = ArenaConsoleSpriteGenerator::GenerateArenaSprites();

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
   inputConfig->KeyMap[KeyCode::Tab] = GameButton::Select;

   inputConfig->KeyMap[KeyCode::A] = GameButton::A;
   inputConfig->KeyMap[KeyCode::Return] = GameButton::A;
   inputConfig->KeyMap[KeyCode::Space] = GameButton::A;
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

   arenaConfig->DefaultPlayerPositionX = arenaConfig->DefaultTileWidth * 8;
   arenaConfig->DefaultPlayerPositionY = arenaConfig->DefaultTileHeight * 6;

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

   config->RenderConfig = BuildConsoleRenderConfig();
   config->InputConfig = BuildKeyboardInputConfig();
   config->PlayerConfig = BuildPlayerConfig();
   config->ArenaConfig = BuildArenaConfig();
   config->PlayerPhysicsConfig = BuildPlayerPhysicsConfig();

   return config;
}
