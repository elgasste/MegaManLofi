#include "ArenaPhysics.h"
#include "IFrameRateProvider.h"
#include "IFrameActionRegistry.h"
#include "IGameEventAggregator.h"
#include "ArenaDefs.h"
#include "IPlayer.h"
#include "IArena.h"
#include "FrameAction.h"
#include "GameEvent.h"

using namespace std;
using namespace MegaManLofi;

ArenaPhysics::ArenaPhysics( const shared_ptr<IFrameRateProvider> frameRateProvider,
                            const shared_ptr<IFrameActionRegistry> frameActionRegistry,
                            const shared_ptr<IGameEventAggregator> eventAggregator,
                            const shared_ptr<ArenaDefs> arenaDefs ) :
   _frameRateProvider( frameRateProvider ),
   _frameActionRegistry( frameActionRegistry ),
   _eventAggregator( eventAggregator ),
   _arenaDefs( arenaDefs ),
   _arena( nullptr )
{
}

void ArenaPhysics::AssignTo( const shared_ptr<IArena> arena )
{
   _arena = arena;

   UpdatePlayerOccupyingTileIndices();
}

void ArenaPhysics::Tick()
{
   MovePlayer();
}

void ArenaPhysics::MovePlayer()
{
   auto player = _arena->GetPlayer();

   if ( player->GetVelocityX() != 0 )
   {
      MovePlayerX();
   }

   if ( player->GetVelocityY() != 0 )
   {
      MovePlayerY();
   }

   UpdatePlayerOccupyingTileIndices();
   UpdateActiveRegion();

   if ( !DetectTileDeath() )
   {
      DetectPlayerStanding();
   }
}

void ArenaPhysics::UpdatePlayerOccupyingTileIndices()
{
   auto player = _arena->GetPlayer();
   const auto& hitBox = player->GetHitBox();
   auto playerPosition = player->GetArenaPosition();

   _playerOccupyingTileIndices.Left = ( playerPosition.Left + hitBox.Left ) / _arena->GetTileWidth();
   _playerOccupyingTileIndices.Top = ( playerPosition.Top + hitBox.Top ) / _arena->GetTileHeight();
   _playerOccupyingTileIndices.Right = (long long)( ( playerPosition.Left + hitBox.Left + hitBox.Width ) / _arena->GetTileWidth() );
   _playerOccupyingTileIndices.Bottom = (long long)( ( playerPosition.Top + hitBox.Top + hitBox.Height ) / _arena->GetTileHeight() );

   // when the player is positioned exactly at the edge of the right or bottom tile,
   // these indices will be incorrect, and need to be decremented.
   if ( ( ( playerPosition.Left + hitBox.Left + hitBox.Width ) % _arena->GetTileWidth() ) == 0 )
   {
      _playerOccupyingTileIndices.Right--;
   }
   if ( ( ( playerPosition.Top + hitBox.Top + hitBox.Height ) % _arena->GetTileHeight() ) == 0 )
   {
      _playerOccupyingTileIndices.Bottom--;
   }
}

void ArenaPhysics::MovePlayerX()
{
   auto player = _arena->GetPlayer();
   auto currentPositionLeft = player->GetArenaPositionLeft();
   auto newPositionLeft = currentPositionLeft + (long long)( player->GetVelocityX() * _frameRateProvider->GetSecondsPerFrame() );
   DetectPlayerTileCollisionX( newPositionLeft );

   if ( currentPositionLeft != newPositionLeft )
   {
      player->SetArenaPositionLeft( newPositionLeft );
      _frameActionRegistry->FlagAction( FrameAction::PlayerMovedHorizontal );
   }
}

void ArenaPhysics::MovePlayerY()
{
   auto player = _arena->GetPlayer();
   auto currentPositionTop = player->GetArenaPositionTop();
   auto newPositionTop = currentPositionTop + (long long)( player->GetVelocityY() * _frameRateProvider->GetSecondsPerFrame() );
   DetectPlayerTileCollisionY( newPositionTop );

   if ( currentPositionTop != newPositionTop )
   {
      player->SetArenaPositionTop( newPositionTop );
      _frameActionRegistry->FlagAction( FrameAction::PlayerMovedVertical );
   }
}

void ArenaPhysics::DetectPlayerTileCollisionX( long long& newPositionLeft )
{
   auto player = _arena->GetPlayer();
   const auto& hitBox = player->GetHitBox();
   auto currentPositionLeft = player->GetArenaPositionLeft();

   // cycle from the top tile we're currently occupying to the bottom tile we're currently occupying
   for ( long long y = _playerOccupyingTileIndices.Top; y <= _playerOccupyingTileIndices.Bottom; y++ )
   {
      if ( newPositionLeft < currentPositionLeft ) // moving left
      {
         auto leftOccupyingTileLeftEdge = _playerOccupyingTileIndices.Left * _arena->GetTileWidth();

         if ( newPositionLeft < 0 )
         {
            // we've collided with the left edge of the arena
            newPositionLeft = 0;
            player->StopX();
            break;
         }
         else if ( newPositionLeft < leftOccupyingTileLeftEdge )
         {
            // check if we're trying to enter a new tile to the left
            const auto& nextLeftTile = _arena->GetTile( ( y * _arena->GetHorizontalTiles() ) + ( _playerOccupyingTileIndices.Left - 1 ) );
            if ( !nextLeftTile.LeftPassable )
            {
               newPositionLeft = leftOccupyingTileLeftEdge;
               player->StopX();
            }
         }
      }
      else // moving right
      {
         auto rightOccupyingTileRightEdge = ( _playerOccupyingTileIndices.Right + 1 ) * _arena->GetTileWidth();
         auto newPositionRight = newPositionLeft + hitBox.Width;
         auto arenaWidth = _arena->GetWidth();

         if ( newPositionRight > arenaWidth )
         {
            // we've collided with the right edge of the arena
            newPositionLeft = arenaWidth - hitBox.Width;
            player->StopX();
            break;
         }
         else if ( newPositionRight > rightOccupyingTileRightEdge )
         {
            // check if we're trying to enter a new tile to the right
            const auto& nextRightTile = _arena->GetTile( ( y * _arena->GetHorizontalTiles() ) + ( _playerOccupyingTileIndices.Right + 1 ) );
            if ( !nextRightTile.RightPassable )
            {
               newPositionLeft = rightOccupyingTileRightEdge - hitBox.Width;
               player->StopX();
            }
         }
      }
   }
}

void ArenaPhysics::DetectPlayerTileCollisionY( long long& newPositionTop )
{
   auto player = _arena->GetPlayer();
   const auto& hitBox = player->GetHitBox();
   auto currentPositionTop = player->GetArenaPositionTop();

   // cycle from the left tile we're currently occupying to the right tile we're currently occupying
   for ( long long x = _playerOccupyingTileIndices.Left; x <= _playerOccupyingTileIndices.Right; x++ )
   {
      if ( newPositionTop < currentPositionTop ) // moving up
      {
         auto topOccupyingTileTopEdge = _playerOccupyingTileIndices.Top * _arena->GetTileHeight();

         if ( newPositionTop < 0 )
         {
            // we've collided with the top edge of the arena
            newPositionTop = 0;
            player->StopY();
            break;
         }
         else if ( newPositionTop < topOccupyingTileTopEdge )
         {
            // check if we're trying to enter a new tile upward
            const auto& nextTileUp = _arena->GetTile( ( ( _playerOccupyingTileIndices.Top - 1 ) * _arena->GetHorizontalTiles() ) + x );
            if ( !nextTileUp.UpPassable )
            {
               newPositionTop = topOccupyingTileTopEdge;
               player->StopY();
            }
         }
      }
      else // moving down
      {
         auto bottomOccupyingTileBottomEdge = ( _playerOccupyingTileIndices.Bottom + 1 ) * _arena->GetTileHeight();
         auto newPositionBottom = newPositionTop + hitBox.Height;
         auto arenaHeight = _arena->GetHeight();

         if ( ( newPositionBottom ) > arenaHeight )
         {
            // we've collided with the bottom edge of the arena
            newPositionTop = arenaHeight - hitBox.Height;
            _eventAggregator->RaiseEvent( GameEvent::Pitfall );
            break;
         }
         else if ( newPositionBottom > bottomOccupyingTileBottomEdge )
         {
            // check if we're trying to enter a new tile downward
            const auto& nextTileDown = _arena->GetTile( ( ( _playerOccupyingTileIndices.Bottom + 1 ) * _arena->GetHorizontalTiles() ) + x );
            if ( !nextTileDown.DownPassable )
            {
               newPositionTop = bottomOccupyingTileBottomEdge - hitBox.Height;
               player->StopY();
            }
         }
      }
   }
}

bool ArenaPhysics::DetectTileDeath() const
{
   for ( long long x = _playerOccupyingTileIndices.Left; x <= _playerOccupyingTileIndices.Right; x++ )
   {
      for ( long long y = _playerOccupyingTileIndices.Top; y <= _playerOccupyingTileIndices.Bottom; y++ )
      {
         auto tile = _arena->GetTile( ( y * _arena->GetHorizontalTiles() ) + x );

         if ( tile.CausesDeath )
         {
            _eventAggregator->RaiseEvent( GameEvent::TileDeath );
            return true;
         }
      }
   }

   return false;
}

void ArenaPhysics::UpdateActiveRegion()
{
   const auto& playerPosition = _arena->GetPlayer()->GetArenaPosition();
   auto regionLeft = playerPosition.Left - ( _arenaDefs->ActiveRegionWidth / 2 );
   auto regionTop = playerPosition.Top - ( _arenaDefs->ActiveRegionHeight / 2 );

   _arena->SetActiveRegion( { regionLeft, regionTop, regionLeft + _arenaDefs->ActiveRegionWidth, regionTop + _arenaDefs->ActiveRegionHeight } );
}

void ArenaPhysics::DetectPlayerStanding()
{
   auto player = _arena->GetPlayer();
   const auto& hitBox = player->GetHitBox();
   auto playerPositionTop = player->GetArenaPositionTop();
   auto playerHitBoxBottom = playerPositionTop + hitBox.Top + hitBox.Height;

   player->SetIsStanding( false );

   if ( playerHitBoxBottom >= _arena->GetHeight() )
   {
      // TODO: what's this? wouldn't this indicate a pitfall?
      player->SetIsStanding( true );
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
         player->SetIsStanding( true );
         return;
      }
   }
}
