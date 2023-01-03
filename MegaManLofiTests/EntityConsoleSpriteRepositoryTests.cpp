#include "gtest/gtest.h"

#include <MegaManLofi/EntityConsoleSpriteRepository.h>
#include <MegaManLofi/GameEventAggregator.h>
#include <MegaManLofi/ConsoleSpriteDefs.h>
#include <MegaManLofi/GameEvent.h>

#include "mock_ReadOnlyArena.h"
#include "mock_EntityConsoleSpriteCopier.h"
#include "mock_ReadOnlyEntity.h"
#include "mock_EntityConsoleSprite.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class EntityConsoleSpriteRepositoryTests : public Test
{
public:
   void SetUp() override
   {
      _eventAggregator.reset( new GameEventAggregator );
      _arenaMock.reset( new NiceMock<mock_ReadOnlyArena> );
      _spriteCopier.reset( new NiceMock<mock_EntityConsoleSpriteCopier> );
      _spriteDefs.reset( new ConsoleSpriteDefs );
      _entityMock1.reset( new NiceMock<mock_ReadOnlyEntity> );
      _entityMock2.reset( new NiceMock<mock_ReadOnlyEntity> );
      _entityMock3.reset( new NiceMock<mock_ReadOnlyEntity> );
      _spriteMock1.reset( new NiceMock<mock_EntityConsoleSprite> );
      _spriteMock2.reset( new NiceMock<mock_EntityConsoleSprite> );
      _spriteMock3.reset( new NiceMock<mock_EntityConsoleSprite> );
      _spriteCopyMock1.reset( new NiceMock<mock_EntityConsoleSprite> );
      _spriteCopyMock2.reset( new NiceMock<mock_EntityConsoleSprite> );
      _spriteCopyMock3.reset( new NiceMock<mock_EntityConsoleSprite> );

      ON_CALL( *_entityMock1, GetUniqueId() ).WillByDefault( Return( 10 ) );
      ON_CALL( *_entityMock1, GetEntityMetaId() ).WillByDefault( Return( 20 ) );

      ON_CALL( *_entityMock2, GetUniqueId() ).WillByDefault( Return( 11 ) );
      ON_CALL( *_entityMock2, GetEntityMetaId() ).WillByDefault( Return( 21 ) );

      ON_CALL( *_entityMock3, GetUniqueId() ).WillByDefault( Return( 12 ) );
      ON_CALL( *_entityMock3, GetEntityMetaId() ).WillByDefault( Return( 22 ) );

      _spriteDefs->EntitySpriteMap[20] = _spriteMock1;
      _spriteDefs->EntitySpriteMap[21] = _spriteMock2;
      _spriteDefs->EntitySpriteMap[22] = _spriteMock3;

      ON_CALL( *_spriteCopier, MakeCopy( static_pointer_cast<IEntityConsoleSprite>( _spriteMock1 ) ) ).WillByDefault( Return( _spriteCopyMock1 ) );
      ON_CALL( *_spriteCopier, MakeCopy( static_pointer_cast<IEntityConsoleSprite>( _spriteMock2 ) ) ).WillByDefault( Return( _spriteCopyMock2 ) );
      ON_CALL( *_spriteCopier, MakeCopy( static_pointer_cast<IEntityConsoleSprite>( _spriteMock3 ) ) ).WillByDefault( Return( _spriteCopyMock3 ) );

      _repository.reset( new EntityConsoleSpriteRepository( _eventAggregator, _arenaMock, _spriteCopier, _spriteDefs ) );
   }

protected:
   shared_ptr<GameEventAggregator> _eventAggregator;
   shared_ptr<mock_ReadOnlyArena> _arenaMock;
   shared_ptr<mock_EntityConsoleSpriteCopier> _spriteCopier;
   shared_ptr<ConsoleSpriteDefs> _spriteDefs;

   shared_ptr<mock_ReadOnlyEntity> _entityMock1;
   shared_ptr<mock_ReadOnlyEntity> _entityMock2;
   shared_ptr<mock_ReadOnlyEntity> _entityMock3;

   shared_ptr<mock_EntityConsoleSprite> _spriteMock1;
   shared_ptr<mock_EntityConsoleSprite> _spriteMock2;
   shared_ptr<mock_EntityConsoleSprite> _spriteMock3;

   shared_ptr<mock_EntityConsoleSprite> _spriteCopyMock1;
   shared_ptr<mock_EntityConsoleSprite> _spriteCopyMock2;
   shared_ptr<mock_EntityConsoleSprite> _spriteCopyMock3;

   shared_ptr<EntityConsoleSpriteRepository> _repository;
};

TEST_F( EntityConsoleSpriteRepositoryTests, EntitySpawned_DoNotHaveMatchingSprite_AddsSpriteToMap )
{
   ON_CALL( *_arenaMock, GetEntityCount() ).WillByDefault( Return( 1 ) );
   ON_CALL( *_arenaMock, GetEntity( 0 ) ).WillByDefault( Return( _entityMock1 ) );

   EXPECT_CALL( *_spriteCopier, MakeCopy( static_pointer_cast<IEntityConsoleSprite>( _spriteMock1 ) ) );
   EXPECT_CALL( *_spriteCopyMock1, AssignTo( static_pointer_cast<ReadOnlyEntity>( _entityMock1 ) ) );

   _eventAggregator->RaiseEvent( GameEvent::ArenaEntitySpawned );

   EXPECT_EQ( _repository->GetSpriteCount(), 1 );
   EXPECT_EQ( _repository->GetSprite( 10 ), _spriteCopyMock1 );
}

TEST_F( EntityConsoleSpriteRepositoryTests, EntitySpawned_AlreadyHaveMatchingSprite_DoesNotAddSpriteToMap )
{
   ON_CALL( *_arenaMock, GetEntityCount() ).WillByDefault( Return( 1 ) );
   ON_CALL( *_arenaMock, GetEntity( 0 ) ).WillByDefault( Return( _entityMock1 ) );

   _eventAggregator->RaiseEvent( GameEvent::ArenaEntitySpawned );
   EXPECT_EQ( _repository->GetSprite( 10 ), _spriteCopyMock1 );

   EXPECT_CALL( *_spriteCopier, MakeCopy( _ ) ).Times( 0 );
   _eventAggregator->RaiseEvent( GameEvent::ArenaEntitySpawned );

   EXPECT_EQ( _repository->GetSpriteCount(), 1 );
}

TEST_F( EntityConsoleSpriteRepositoryTests, EntityDeSpawned_HaveMatchingSprite_RemovesSpriteFromMap )
{
   // arena has three entities
   ON_CALL( *_arenaMock, GetEntityCount() ).WillByDefault( Return( 3 ) );
   ON_CALL( *_arenaMock, GetEntity( 0 ) ).WillByDefault( Return( _entityMock1 ) );
   ON_CALL( *_arenaMock, GetEntity( 1 ) ).WillByDefault( Return( _entityMock2 ) );
   ON_CALL( *_arenaMock, GetEntity( 2 ) ).WillByDefault( Return( _entityMock3 ) );

   // should add three sprites
   _eventAggregator->RaiseEvent( GameEvent::ArenaEntitySpawned );
   EXPECT_EQ( _repository->GetSpriteCount(), 3 );
   EXPECT_EQ( _repository->GetSprite( 10 ), _spriteCopyMock1 );
   EXPECT_EQ( _repository->GetSprite( 11 ), _spriteCopyMock2 );
   EXPECT_EQ( _repository->GetSprite( 12 ), _spriteCopyMock3 );

   // first two entities are gone from arena
   ON_CALL( *_arenaMock, HasEntity( 10 ) ).WillByDefault( Return( false ) );
   ON_CALL( *_arenaMock, HasEntity( 11 ) ).WillByDefault( Return( false ) );
   ON_CALL( *_arenaMock, HasEntity( 12 ) ).WillByDefault( Return( true ) );

   // should remove first two sprites
   _eventAggregator->RaiseEvent( GameEvent::ArenaEntityDeSpawned );
   EXPECT_EQ( _repository->GetSpriteCount(), 1 );
   EXPECT_EQ( _repository->GetSprite( 12 ), _spriteCopyMock3 );
}
