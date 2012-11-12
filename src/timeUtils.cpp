#include "timeUtils.h"
#include <string>
#include <time.h>
#define DAY_SECONDS 24*60*60
using namespace std;

unsigned int TimeUtils::getCurrentTime()
{
	return (unsigned int)time(NULL);
}

unsigned int TimeUtils::getTimeStamp(int dayBack)
{
	time_t timer		= time(NULL);
	unsigned int interval	= dayBack*DAY_SECONDS;
	return ((unsigned int)timer - interval);
}
