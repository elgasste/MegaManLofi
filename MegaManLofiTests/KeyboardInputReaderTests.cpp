#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/KeyboardInputReader.h>
#include <MegaManLofi/KeyboardInputDefs.h>
#include <MegaManLofi/KeyCode.h>
#include <MegaManLofi/GameButton.h>

#include "mock_Keyboard.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class KeyboardInputReaderTests : public Test
{
public:
   void SetUp() override
   {
      _inputDefs.reset( new KeyboardInputDefs );
      _keyboardMock.reset( new NiceMock<mock_Keyboard> );
   }

   void AddKeyBinding( KeyCode keyCode, GameButton gameButton )
   {
      _inputDefs->KeyMap[keyCode] = gameButton;
   }

   void BuildInputReader()
   {
      _inputReader.reset( new KeyboardInputReader( _inputDefs, _keyboardMock ) );
   }

protected:
   shared_ptr<KeyboardInputDefs> _inputDefs;
   shared_ptr<mock_Keyboard> _keyboardMock;

   shared_ptr<KeyboardInputReader> _inputReader;
};

TEST_F( KeyboardInputReaderTests, ReadInput_ButtonWasPressed_MarksButtonAsDownAndPressed )
{
   AddKeyBinding( KeyCode::A, GameButton::A );
   BuildInputReader();

   EXPECT_CALL( *_keyboardMock, IsKeyDown( KeyCode::A ) ).WillRepeatedly( Return( true ) );

   _inputReader->ReadInput();

   EXPECT_TRUE( _inputReader->WasButtonPressed( GameButton::A ) );
   EXPECT_TRUE( _inputReader->IsButtonDown( GameButton::A ) );
}

TEST_F( KeyboardInputReaderTests, ReadInput_ButtonIsDownAndWasAlreadyPressed_MarksButtonAsDownAndNotPressed )
{
   AddKeyBinding( KeyCode::A, GameButton::A );
   BuildInputReader();

   EXPECT_CALL( *_keyboardMock, IsKeyDown( KeyCode::A ) ).WillRepeatedly( Return( true ) );

   _inputReader->ReadInput();
   _inputReader->ReadInput();

   EXPECT_FALSE( _inputReader->WasButtonPressed( GameButton::A ) );
   EXPECT_TRUE( _inputReader->IsButtonDown( GameButton::A ) );
}

TEST_F( KeyboardInputReaderTests, ReadInput_ButtonWasReleased_MarksButtonAsUpAndNotPressed )
{
   AddKeyBinding( KeyCode::A, GameButton::A );
   BuildInputReader();

   EXPECT_CALL( *_keyboardMock, IsKeyDown( KeyCode::A ) )
      .WillOnce( Return( true ) )
      .WillOnce( Return( true ) )
      .WillRepeatedly( Return( false ) );

   _inputReader->ReadInput();
   _inputReader->ReadInput();
   _inputReader->ReadInput();

   EXPECT_FALSE( _inputReader->WasButtonPressed( GameButton::A ) );
   EXPECT_FALSE( _inputReader->IsButtonDown( GameButton::A ) );
}

TEST_F( KeyboardInputReaderTests, ReadInput_MultipleKeysBoundToButton_AllKeyBindingsWorkCorrectly )
{
   AddKeyBinding( KeyCode::A, GameButton::A );
   AddKeyBinding( KeyCode::B, GameButton::A );
   AddKeyBinding( KeyCode::C, GameButton::A );
   BuildInputReader();

   EXPECT_CALL( *_keyboardMock, IsKeyDown( KeyCode::A ) )
      .WillOnce( Return( true ) )
      .WillRepeatedly( Return( false ) );
   EXPECT_CALL( *_keyboardMock, IsKeyDown( KeyCode::B ) )
      .WillOnce( Return( true ) )
      .WillRepeatedly( Return( false ) );
   EXPECT_CALL( *_keyboardMock, IsKeyDown( KeyCode::C ) )
      .WillOnce( Return( true ) )
      .WillRepeatedly( Return( false ) );

   _inputReader->ReadInput();

   EXPECT_TRUE( _inputReader->WasButtonPressed( GameButton::A ) );
   EXPECT_TRUE( _inputReader->IsButtonDown( GameButton::A ) );

   _inputReader->ReadInput();

   EXPECT_FALSE( _inputReader->WasButtonPressed( GameButton::A ) );
   EXPECT_TRUE( _inputReader->IsButtonDown( GameButton::A ) );

   _inputReader->ReadInput();

   EXPECT_FALSE( _inputReader->WasButtonPressed( GameButton::A ) );
   EXPECT_TRUE( _inputReader->IsButtonDown( GameButton::A ) );

   _inputReader->ReadInput();

   EXPECT_FALSE( _inputReader->WasButtonPressed( GameButton::A ) );
   EXPECT_FALSE( _inputReader->IsButtonDown( GameButton::A ) );
}

TEST_F( KeyboardInputReaderTests, WasAnyButtonPressed_NoButtonWasPressed_ReturnsFalse )
{
   AddKeyBinding( KeyCode::A, GameButton::A );
   BuildInputReader();

   EXPECT_CALL( *_keyboardMock, IsKeyDown( _ ) ).WillRepeatedly( Return( false ) );

   _inputReader->ReadInput();

   EXPECT_FALSE( _inputReader->WasAnyButtonPressed() );
}

TEST_F( KeyboardInputReaderTests, WasAnyButtonPressed_DiagnosticsButtonWasPressed_ReturnsFalse )
{
   AddKeyBinding( KeyCode::F12, GameButton::Diagnostics );
   BuildInputReader();

   EXPECT_CALL( *_keyboardMock, IsKeyDown( KeyCode::F12 ) ).WillRepeatedly( Return( true ) );

   _inputReader->ReadInput();

   EXPECT_FALSE( _inputReader->WasAnyButtonPressed() );
}

TEST_F( KeyboardInputReaderTests, WasAnyButtonPressed_NonDiagnosticsButtonWasPressed_ReturnsTrue )
{
   AddKeyBinding( KeyCode::A, GameButton::A );
   BuildInputReader();

   EXPECT_CALL( *_keyboardMock, IsKeyDown( KeyCode::A ) ).WillRepeatedly( Return( true ) );

   _inputReader->ReadInput();

   EXPECT_TRUE( _inputReader->WasAnyButtonPressed() );
}
