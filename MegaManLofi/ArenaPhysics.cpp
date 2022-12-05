#include "ArenaPhysics.h"
#include "IFrameRateProvider.h"
#include "IFrameActionRegistry.h"
#include "IPlayer.h"
#include "IArena.h"
#include "FrameAction.h"

using namespace std;
using namespace MegaManLofi;

ArenaPhysics::ArenaPhysics( const shared_ptr<IFrameRateProvider> frameRateProvider,
                            const shared_ptr<IFrameActionRegistry> frameActionRegistry,
                            const shared_ptr<IArena> arena,
                            const shared_ptr<IPlayer> player ) :
   _frameRateProvider( frameRateProvider ),
   _frameActionRegistry( frameActionRegistry ),
   _arena( arena ),
   _player( player )
{
   UpdatePlayerOccupyingTileIndices();
}

void ArenaPhysics::Tick()
{
   MovePlayer();
}

void ArenaPhysics::MovePlayer()
{
   if ( _player->GetVelocityX() != 0 )
   {
      MovePlayerX();
   }

   if ( _player->GetVelocityY() != 0 )
   {
      MovePlayerY();
   }

   UpdatePlayerOccupyingTileIndices();
   DetectPlayerStanding();
}

void ArenaPhysics::UpdatePlayerOccupyingTileIndices()
{
   const auto& hitBox = _player->GetHitBox();
   auto playerPositionX = _arena->GetPlayerPositionX();
   auto playerPositionY = _arena->GetPlayerPositionY();

   _playerOccupyingTileIndices.Left = ( playerPositionX + hitBox.Left ) / _arena->GetTileWidth();
   _playerOccupyingTileIndices.Top = ( playerPositionY + hitBox.Top ) / _arena->GetTileHeight();
   _playerOccupyingTileIndices.Right = (long long)( ( playerPositionX + hitBox.Left + hitBox.Width ) / _arena->GetTileWidth() );
   _playerOccupyingTileIndices.Bottom = (long long)( ( playerPositionY + hitBox.Top + hitBox.Height ) / _arena->GetTileHeight() );

   // when the player is positioned exactly at the edge of the right or bottom tile,
   // these indices will be incorrect, and need to be decremented.
   if ( ( ( playerPositionX + hitBox.Left + hitBox.Width ) % _arena->GetTileWidth() ) == 0 )
   {
      _playerOccupyingTileIndices.Right--;
   }
   if ( ( ( playerPositionY + hitBox.Top + hitBox.Height ) % _arena->GetTileHeight() ) == 0 )
   {
      _playerOccupyingTileIndices.Bottom--;
   }
}

void ArenaPhysics::MovePlayerX()
{
   auto currentPositionX = _arena->GetPlayerPositionX();
   auto newPositionX = currentPositionX + ( _player->GetVelocityX() / _frameRateProvider->GetFramesPerSecond() );
   DetectPlayerTileCollisionX( newPositionX );

   if ( currentPositionX != newPositionX )
   {
      _arena->SetPlayerPositionX( newPositionX );
      _frameActionRegistry->FlagAction( FrameAction::PlayerMovedHorizontal );
   }
}

void ArenaPhysics::MovePlayerY()
{
   auto currentPositionY = _arena->GetPlayerPositionY();
   auto newPositionY = currentPositionY + ( _player->GetVelocityY() / _frameRateProvider->GetFramesPerSecond() );
   DetectPlayerTileCollisionY( newPositionY );

   if ( currentPositionY != newPositionY )
   {
      _arena->SetPlayerPositionY( newPositionY );
      _frameActionRegistry->FlagAction( FrameAction::PlayerMovedVertical );
   }
}

void ArenaPhysics::DetectPlayerTileCollisionX( long long& newPositionX )
{
   const auto& hitBox = _player->GetHitBox();
   auto currentPositionX = _arena->GetPlayerPositionX();

   // cycle from the top tile we're currently occupying to the bottom tile we're currently occupying
   for ( long long y = _playerOccupyingTileIndices.Top; y <= _playerOccupyingTileIndices.Bottom; y++ )
   {
      if ( newPositionX < currentPositionX ) // moving left
      {
         auto leftOccupyingTileLeftEdge = _playerOccupyingTileIndices.Left * _arena->GetTileWidth();

         if ( newPositionX < 0 )
         {
            // we've collided with the left edge of the arena
            newPositionX = 0;
            _player->StopX();
            break;
         }
         else if ( newPositionX < leftOccupyingTileLeftEdge )
         {
            // check if we're trying to enter a new tile to the left
            const auto& nextLeftTile = _arena->GetTile( ( y * _arena->GetHorizontalTiles() ) + ( _playerOccupyingTileIndices.Left - 1 ) );
            if ( !nextLeftTile.LeftPassable )
            {
               newPositionX = leftOccupyingTileLeftEdge;
               _player->StopX();
            }
         }
      }
      else // moving right
      {
         auto rightOccupyingTileRightEdge = ( _playerOccupyingTileIndices.Right + 1 ) * _arena->GetTileWidth();
         auto newPositionXRight = newPositionX + hitBox.Width;
         auto arenaWidth = _arena->GetWidth();

         if ( newPositionXRight > arenaWidth )
         {
            // we've collided with the right edge of the arena
            newPositionX = arenaWidth - hitBox.Width;
            _player->StopX();
            break;
         }
         else if ( newPositionXRight > rightOccupyingTileRightEdge )
         {
            // check if we're trying to enter a new tile to the right
            const auto& nextRightTile = _arena->GetTile( ( y * _arena->GetHorizontalTiles() ) + ( _playerOccupyingTileIndices.Right + 1 ) );
            if ( !nextRightTile.RightPassable )
            {
               newPositionX = rightOccupyingTileRightEdge - hitBox.Width;
               _player->StopX();
            }
         }
      }
   }
}

void ArenaPhysics::DetectPlayerTileCollisionY( long long& newPositionY )
{
   const auto& hitBox = _player->GetHitBox();
   auto currentPositionY = _arena->GetPlayerPositionY();

   // cycle from the left tile we're currently occupying to the right tile we're currently occupying
   for ( long long x = _playerOccupyingTileIndices.Left; x <= _playerOccupyingTileIndices.Right; x++ )
   {
      if ( newPositionY < currentPositionY ) // moving up
      {
         auto topOccupyingTileTopEdge = _playerOccupyingTileIndices.Top * _arena->GetTileHeight();

         if ( newPositionY < 0 )
         {
            // we've collided with the top edge of the arena
            newPositionY = 0;
            _player->StopY();
            break;
         }
         else if ( newPositionY < topOccupyingTileTopEdge )
         {
            // check if we're trying to enter a new tile upward
            const auto& nextTileUp = _arena->GetTile( ( ( _playerOccupyingTileIndices.Top - 1 ) * _arena->GetHorizontalTiles() ) + x );
            if ( !nextTileUp.UpPassable )
            {
               newPositionY = topOccupyingTileTopEdge;
               _player->StopY();
            }
         }
      }
      else // moving down
      {
         auto bottomOccupyingTileBottomEdge = ( _playerOccupyingTileIndices.Bottom + 1 ) * _arena->GetTileHeight();
         auto newPositionYBottom = newPositionY + hitBox.Height;
         auto arenaHeight = _arena->GetHeight();

         if ( ( newPositionYBottom ) > arenaHeight )
         {
            // we've collided with the bottom edge of the arena
            newPositionY = arenaHeight - hitBox.Height;
            _player->StopY();
            break;
         }
         else if ( newPositionYBottom > bottomOccupyingTileBottomEdge )
         {
            // check if we're trying to enter a new tile downward
            const auto& nextTileDown = _arena->GetTile( ( ( _playerOccupyingTileIndices.Bottom + 1 ) * _arena->GetHorizontalTiles() ) + x );
            if ( !nextTileDown.DownPassable )
            {
               newPositionY = bottomOccupyingTileBottomEdge - hitBox.Height;
               _player->StopY();
            }
         }
      }
   }
}

void ArenaPhysics::DetectPlayerStanding()
{
   const auto& hitBox = _player->GetHitBox();
   auto playerPositionY = _arena->GetPlayerPositionY();
   auto playerHitBoxBottom = playerPositionY + hitBox.Top + hitBox.Height;

   _player->SetIsStanding( false );

   if ( playerHitBoxBottom >= _arena->GetHeight() )
   {
      _player->SetIsStanding( true );
      return;
   }
   else if ( ( playerHitBoxBottom % _arena->GetTileHeight() ) != 0 )
   {
      return;
   }

   for ( long long x = _playerOccupyingTileIndices.Left; x <= _playerOccupyingTileIndices.Right; x++ )
   {
      const auto& nextTileDown = _arena->GetTile( ( ( _playerOccupyingTileIndices.Bottom + 1 ) * _arena->GetHorizontalTiles() ) + x );

      if ( !nextTileDown.DownPassable )
      {
         _player->SetIsStanding( true );
         return;
      }
   }
}
