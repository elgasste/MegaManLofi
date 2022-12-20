#include "ConsoleAnimationRepository.h"

using namespace std;
using namespace MegaManLofi;

void ConsoleAnimationRepository::AddAnimation( ConsoleAnimationType type, const shared_ptr<IConsoleAnimation> animation )
{
   _animationMap[type] = animation;
}

const shared_ptr<IConsoleAnimation> ConsoleAnimationRepository::GetAnimation( ConsoleAnimationType type ) const
{
   return _animationMap.at( type );
}
