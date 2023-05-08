#include <random>

#include "RandomWrapper.h"

using namespace std;
using namespace MegaManLofi;

namespace
{
   random_device r_device;
   mt19937 rng( r_device() );
}

int RandomWrapper::GetInt( int min, int max ) const
{
   uniform_int_distribution<int> distribution( min, max );
   return distribution( r_device );
}

float RandomWrapper::GetFloat( float min, float max ) const
{
   uniform_real_distribution<float> distribution( min, max );
   return distribution( r_device );
}
