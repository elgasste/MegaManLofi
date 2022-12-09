#include "PlayingStateInputHandler.h"
#include "IGameInputReader.h"
#include "IGameCommandExecutor.h"
#include "GameButton.h"
#include "GameCommand.h"
#include "PushPlayerCommandArgs.h"
#include "PointPlayerCommandArgs.h"
#include "Direction.h"

using namespace std;
using namespace MegaManLofi;

PlayingStateInputHandler::PlayingStateInputHandler( const shared_ptr<IGameInputReader> inputReader,
                                                    const shared_ptr<IGameCommandExecutor> commandExecutor ) :
   _inputReader( inputReader ),
   _commandExecutor( commandExecutor )
{
}

void PlayingStateInputHandler::HandleInput()
{
   if ( _inputReader->WasButtonPressed( GameButton::Start ) )
   {
      _commandExecutor->ExecuteCommand( GameCommand::TogglePause );
   }

   if ( _inputReader->WasButtonPressed( GameButton::Select ) )
   {
      _commandExecutor->ExecuteCommand( GameCommand::Quit );
   }

   if ( _inputReader->WasButtonPressed( GameButton::A ) )
   {
      _commandExecutor->ExecuteCommand( GameCommand::Jump );
   }
   else if ( _inputReader->IsButtonDown( GameButton::A ) )
   {
      _commandExecutor->ExecuteCommand( GameCommand::ExtendJump );
   }

   HandleDirections();
}

void PlayingStateInputHandler::HandleDirections()
{
   bool leftDown = _inputReader->IsButtonDown( GameButton::Left );
   bool upDown = _inputReader->IsButtonDown( GameButton::Up );
   bool rightDown = _inputReader->IsButtonDown( GameButton::Right );
   bool downDown = _inputReader->IsButtonDown( GameButton::Down );

   if ( leftDown )
   {
      _commandExecutor->ExecuteCommand( GameCommand::PushPlayer,
                                        shared_ptr<PushPlayerCommandArgs>( new PushPlayerCommandArgs( Direction::Left ) ) );

      if ( upDown )
      {
         _commandExecutor->ExecuteCommand( GameCommand::PointPlayer,
                                           shared_ptr<PointPlayerCommandArgs>( new PointPlayerCommandArgs( Direction::UpLeft ) ) );
      }
      else if ( downDown )
      {
         _commandExecutor->ExecuteCommand( GameCommand::PointPlayer,
                                           shared_ptr<PointPlayerCommandArgs>( new PointPlayerCommandArgs( Direction::DownLeft ) ) );
      }
      else
      {
         _commandExecutor->ExecuteCommand( GameCommand::PointPlayer,
                                           shared_ptr<PointPlayerCommandArgs>( new PointPlayerCommandArgs( Direction::Left ) ) );
      }
   }
   else if ( rightDown )
   {
      _commandExecutor->ExecuteCommand( GameCommand::PushPlayer,
                                        shared_ptr<PushPlayerCommandArgs>( new PushPlayerCommandArgs( Direction::Right ) ) );

      if ( upDown )
      {
         _commandExecutor->ExecuteCommand( GameCommand::PointPlayer,
                                           shared_ptr<PushPlayerCommandArgs>( new PushPlayerCommandArgs( Direction::UpRight ) ) );
      }
      else if ( downDown )
      {
         _commandExecutor->ExecuteCommand( GameCommand::PointPlayer,
                                           shared_ptr<PointPlayerCommandArgs>( new PointPlayerCommandArgs( Direction::DownRight ) ) );
      }
      else
      {
         _commandExecutor->ExecuteCommand( GameCommand::PointPlayer,
                                           shared_ptr<PointPlayerCommandArgs>( new PointPlayerCommandArgs( Direction::Right ) ) );
      }
   }
   else if ( upDown )
   {
      _commandExecutor->ExecuteCommand( GameCommand::PointPlayer,
                                        shared_ptr<PointPlayerCommandArgs>( new PointPlayerCommandArgs( Direction::Up ) ) );
   }
   else if ( downDown )
   {
      _commandExecutor->ExecuteCommand( GameCommand::PointPlayer,
                                        shared_ptr<PointPlayerCommandArgs>( new PointPlayerCommandArgs( Direction::Down ) ) );
   }
}
