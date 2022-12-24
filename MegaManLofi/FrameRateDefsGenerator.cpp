#include "FrameRateDefsGenerator.h"
#include "FrameRateDefs.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<FrameRateDefs> FrameRateDefsGenerator::GenerateFrateRateDefs()
{
   auto frameRateDefs = make_shared<FrameRateDefs>();

   frameRateDefs->DefaultFramesPerSecond = 60;

   return frameRateDefs;
}
