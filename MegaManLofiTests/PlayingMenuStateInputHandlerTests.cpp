#include "gtest/gtest.h"

#include <memory>

#include <MegaManLofi/PlayingMenuStateInputHandler.h>

#include "mock_GameInputReader.h"
#include "mock_GameCommandExecutor.h"

using namespace std;
using namespace testing;
using namespace MegaManLofi;

class PlayingMenuStateInputHandlerTests : public Test
{
public:
   void SetUp() override
   {
      _inputReaderMock.reset( new NiceMock<mock_GameInputReader> );
      _commandExecutorMock.reset( new NiceMock<mock_GameCommandExecutor> );

      _inputHandler.reset( new PlayingMenuStateInputHandler( _inputReaderMock,
                                                             _commandExecutorMock ) );

      ON_CALL( *_inputReaderMock, WasButtonPressed( _ ) ).WillByDefault( Return( false ) );
      ON_CALL( *_inputReaderMock, IsButtonDown( _ ) ).WillByDefault( Return( false ) );
   }

protected:
   shared_ptr<mock_GameInputReader> _inputReaderMock;
   shared_ptr<mock_GameCommandExecutor> _commandExecutorMock;

   shared_ptr<PlayingMenuStateInputHandler> _inputHandler;
};
