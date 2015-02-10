#include "BateriaDeTestes.h"
#include "TimeSeries.h"
#include "CommonDebug.h"

using namespace std;

int main()
{
	BateriaDeTestes bSmall;
    BateriaDeTestes bLarge;
	
    srand(time(NULL));
	
    bSmall.run(SMALL_SIZE);
    bLarge.run(LARGE_SIZE);
	printf("MAIN EXIT\n");

	return 0;
}
