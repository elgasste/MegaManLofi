#include "UniqueNumberGenerator.h"

using namespace MegaManLofi;

UniqueNumberGenerator::UniqueNumberGenerator() :
   _nextNumber( -1 )
{
}

int UniqueNumberGenerator::GetNext()
{
   _nextNumber++;
   return _nextNumber;
}
