#include <random>

#include "RandomWrapper.h"

using namespace std;
using namespace MegaManLofi;

namespace
{
   random_device r_device;
   mt19937 rng( r_device() );
   uniform_int_distribution<mt19937::result_type> r_distribution;
}

unsigned int RandomWrapper::GetUnsignedInt( unsigned int min, unsigned int max ) const
{
   return ( r_distribution( r_device ) % ( ( max + 1 ) - min ) ) + min;
}
