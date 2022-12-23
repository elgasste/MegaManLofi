#include "KeyboardInputDefsGenerator.h"
#include "KeyboardInputDefs.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<KeyboardInputDefs> KeyboardInputDefsGenerator::GenerateKeyboardInputDefs()
{
   auto keyboardInputDefs = make_shared<KeyboardInputDefs>();

   // key code bindings
   keyboardInputDefs->KeyMap[KeyCode::Left] = GameButton::Left;
   keyboardInputDefs->KeyMap[KeyCode::Up] = GameButton::Up;
   keyboardInputDefs->KeyMap[KeyCode::Right] = GameButton::Right;
   keyboardInputDefs->KeyMap[KeyCode::Down] = GameButton::Down;

   keyboardInputDefs->KeyMap[KeyCode::Return] = GameButton::Start;
   keyboardInputDefs->KeyMap[KeyCode::RShiftKey] = GameButton::Select;

   keyboardInputDefs->KeyMap[KeyCode::A] = GameButton::A;
   keyboardInputDefs->KeyMap[KeyCode::B] = GameButton::B;
   keyboardInputDefs->KeyMap[KeyCode::X] = GameButton::X;
   keyboardInputDefs->KeyMap[KeyCode::Y] = GameButton::Y;

   keyboardInputDefs->KeyMap[KeyCode::NumPad1] = GameButton::L1;
   keyboardInputDefs->KeyMap[KeyCode::NumPad2] = GameButton::L2;
   keyboardInputDefs->KeyMap[KeyCode::NumPad3] = GameButton::R1;
   keyboardInputDefs->KeyMap[KeyCode::NumPad4] = GameButton::R2;

   keyboardInputDefs->KeyMap[KeyCode::F12] = GameButton::Diagnostics;

   // key names
   keyboardInputDefs->KeyNames[KeyCode::Left] = "Left Arrow";
   keyboardInputDefs->KeyNames[KeyCode::Up] = "Up Arrow";
   keyboardInputDefs->KeyNames[KeyCode::Right] = "Right Arrow";
   keyboardInputDefs->KeyNames[KeyCode::Down] = "Down Arrow";
   keyboardInputDefs->KeyNames[KeyCode::Return] = "Enter";
   keyboardInputDefs->KeyNames[KeyCode::LShiftKey] = "Left Shift";
   keyboardInputDefs->KeyNames[KeyCode::RShiftKey] = "Right Shift";
   keyboardInputDefs->KeyNames[KeyCode::Space] = "Space Bar";
   keyboardInputDefs->KeyNames[KeyCode::Tab] = "Tab";
   keyboardInputDefs->KeyNames[KeyCode::A] = "A";
   keyboardInputDefs->KeyNames[KeyCode::B] = "B";
   keyboardInputDefs->KeyNames[KeyCode::X] = "X";
   keyboardInputDefs->KeyNames[KeyCode::Y] = "Y";
   keyboardInputDefs->KeyNames[KeyCode::NumPad1] = "1";
   keyboardInputDefs->KeyNames[KeyCode::NumPad2] = "2";
   keyboardInputDefs->KeyNames[KeyCode::NumPad3] = "3";
   keyboardInputDefs->KeyNames[KeyCode::NumPad4] = "4";
   keyboardInputDefs->KeyNames[KeyCode::F12] = "F12";

   // button names
   keyboardInputDefs->ButtonNames[GameButton::Left] = "Left";
   keyboardInputDefs->ButtonNames[GameButton::Up] = "Up";
   keyboardInputDefs->ButtonNames[GameButton::Right] = "Right";
   keyboardInputDefs->ButtonNames[GameButton::Down] = "Down";
   keyboardInputDefs->ButtonNames[GameButton::Start] = "Start";
   keyboardInputDefs->ButtonNames[GameButton::Select] = "Select";
   keyboardInputDefs->ButtonNames[GameButton::A] = "A";
   keyboardInputDefs->ButtonNames[GameButton::B] = "B";
   keyboardInputDefs->ButtonNames[GameButton::X] = "X";
   keyboardInputDefs->ButtonNames[GameButton::Y] = "Y";
   keyboardInputDefs->ButtonNames[GameButton::L1] = "L1";
   keyboardInputDefs->ButtonNames[GameButton::L2] = "L2";
   keyboardInputDefs->ButtonNames[GameButton::R1] = "R1";
   keyboardInputDefs->ButtonNames[GameButton::R2] = "R2";
   keyboardInputDefs->ButtonNames[GameButton::Diagnostics] = "Toggle Diagnostics";

   return keyboardInputDefs;
}
