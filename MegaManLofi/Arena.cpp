#include "Arena.h"
#include "ArenaConfig.h"
#include "IPlayer.h"
#include "IFrameActionRegistry.h"
#include "IFrameRateProvider.h"
#include "FrameAction.h"
#include "ArenaTile.h"

using namespace std;
using namespace MegaManLofi;

Arena::Arena( const shared_ptr<ArenaConfig> config,
              const shared_ptr<IPlayer> player,
              const shared_ptr<IFrameActionRegistry> frameActionRegistry,
              const shared_ptr<IFrameRateProvider> frameRateProvider ) :
   _config( config ),
   _player( player ),
   _frameActionRegistry( frameActionRegistry ),
   _frameRateProvider( frameRateProvider ),
   _tiles( config->Tiles ),
   _width( config->TileWidth* config->HorizontalTiles ),
   _height( config->TileHeight* config->VerticalTiles ),
   _playerPositionX( config->PlayerStartX ),
   _playerPositionY( config->PlayerStartY )
{
}

void Arena::MovePlayer()
{
   UpdatePlayerOccupyingTileIndices();

   if ( _player->GetVelocityX() != 0. )
   {
      MovePlayerX();
   }

   if ( _player->GetVelocityY() != 0. )
   {
      MovePlayerY();
   }
}

void Arena::UpdatePlayerOccupyingTileIndices()
{
   const auto& hitBox = _player->GetHitBox();

   _playerOccupyingTileIndices.Left = (long long)( ( _playerPositionX + hitBox.Left ) / _config->TileWidth );
   _playerOccupyingTileIndices.Top = (long long)( ( _playerPositionY + hitBox.Top ) / _config->TileHeight );

   // if the hit box is exactly on a right or bottom tile edge, these will be incorrect,
   // so a quick and dirty solution is to trim a tiny bit off the hit box width and height
   // TODO: there must be a better way to do this, because this can still mess up in rare cases...
   // how can we (quickly & easily) tell if the right/bottom edge perfectly matches a tile boundary?
   _playerOccupyingTileIndices.Right = (long long)( ( _playerPositionX + hitBox.Left + ( hitBox.Width - 0.001 ) ) / _config->TileWidth );
   _playerOccupyingTileIndices.Bottom = (long long)( ( _playerPositionY + hitBox.Top + hitBox.Height - 0.001 ) / _config->TileHeight );
}

void Arena::MovePlayerX()
{
   auto newPositionX = _playerPositionX + ( _player->GetVelocityX() / _frameRateProvider->GetFramesPerSecond() );
   DetectPlayerTileCollisionX( newPositionX );

   if ( _playerPositionX != newPositionX )
   {
      _playerPositionX = newPositionX;
      _frameActionRegistry->FlagAction( FrameAction::PlayerMovedHorizontal );
   }
}

void Arena::DetectPlayerTileCollisionX( double& newPositionX )
{
   const auto& hitBox = _player->GetHitBox();

   // cycle from the top tile we're currently occupying to the bottom tile we're currently occupying
   for ( long long y = _playerOccupyingTileIndices.Top; y <= _playerOccupyingTileIndices.Bottom; y++ )
   {
      if ( newPositionX < _playerPositionX ) // moving left
      {
         auto leftOccupyingTileLeftEdge = _playerOccupyingTileIndices.Left * _config->TileWidth;

         if ( newPositionX < 0. )
         {
            // we've collided with the left edge of the arena
            newPositionX = 0.;
            _player->StopX();
            break;
         }
         else if ( newPositionX < leftOccupyingTileLeftEdge )
         {
            // check if we're trying to enter a new tile to the left
            const auto& nextLeftTile = _config->Tiles[( y * _config->HorizontalTiles ) + ( _playerOccupyingTileIndices.Left - 1 )];
            if ( !nextLeftTile.LeftPassable )
            {
               newPositionX = leftOccupyingTileLeftEdge;
               _player->StopX();
            }
         }
      }
      else // moving right
      {
         auto rightOccupyingTileRightEdge = ( _playerOccupyingTileIndices.Right + 1 ) * _config->TileWidth;
         auto newPositionXRight = newPositionX + hitBox.Width;

         if ( newPositionXRight > _width )
         {
            // we've collided with the right edge of the arena
            newPositionX = _width - hitBox.Width;
            _player->StopX();
            break;
         }
         else if ( newPositionXRight > rightOccupyingTileRightEdge )
         {
            // check if we're trying to enter a new tile to the right
            const auto& nextRightTile = _config->Tiles[( y * _config->HorizontalTiles ) + ( _playerOccupyingTileIndices.Right + 1 )];
            if ( !nextRightTile.RightPassable )
            {
               newPositionX = rightOccupyingTileRightEdge - hitBox.Width;
               _player->StopX();
            }
         }
      }
   }
}

void Arena::MovePlayerY()
{
   auto newPositionY = _playerPositionY + ( _player->GetVelocityY() / _frameRateProvider->GetFramesPerSecond() );
   DetectPlayerTileCollisionY( newPositionY );

   if ( _playerPositionY != newPositionY )
   {
      _playerPositionY = newPositionY;
      _frameActionRegistry->FlagAction( FrameAction::PlayerMovedVertical );
   }
}

void Arena::DetectPlayerTileCollisionY( double& newPositionY )
{
   const auto& hitBox = _player->GetHitBox();

   // cycle from the left tile we're currently occupying to the right tile we're currently occupying
   for ( long long x = _playerOccupyingTileIndices.Left; x <= _playerOccupyingTileIndices.Right; x++ )
   {
      if ( newPositionY < _playerPositionY ) // moving up
      {
         auto topOccupyingTileTopEdge = _playerOccupyingTileIndices.Top * _config->TileHeight;

         if ( newPositionY < 0. )
         {
            // we've collided with the top edge of the arena
            newPositionY = 0.;
            _player->StopY();
            break;
         }
         else if ( newPositionY < topOccupyingTileTopEdge )
         {
            // check if we're trying to enter a new tile upward
            const auto& nextTileUp = _config->Tiles[( ( _playerOccupyingTileIndices.Top - 1 ) * _config->HorizontalTiles ) + x];
            if ( !nextTileUp.UpPassable )
            {
               newPositionY = topOccupyingTileTopEdge;
               _player->StopY();
            }
         }
      }
      else // moving down
      {
         auto bottomOccupyingTileBottomEdge = ( _playerOccupyingTileIndices.Bottom + 1 ) * _config->TileHeight;
         auto newPositionYBottom = newPositionY + hitBox.Height;

         if ( ( newPositionYBottom ) > _height )
         {
            // we've collided with the bottom edge of the arena
            newPositionY = _height - hitBox.Height;
            _player->StopY();
            break;
         }
         else if ( newPositionYBottom > bottomOccupyingTileBottomEdge )
         {
            // check if we're trying to enter a new tile downward
            const auto& nextTileDown = _config->Tiles[( ( _playerOccupyingTileIndices.Bottom + 1 ) * _config->HorizontalTiles ) + x];
            if ( !nextTileDown.DownPassable )
            {
               newPositionY = bottomOccupyingTileBottomEdge - hitBox.Height;
               _player->StopY();
            }
         }
      }
   }
}
