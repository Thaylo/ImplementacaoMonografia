#include "BateriaDeTestes.h"
#include "TimeSeries.h"
#include "CommonDebug.h"

using namespace std;

int main()
{
	BateriaDeTestes b;
    
	srand(time(NULL));
	b.run(SMALL_SIZE);
	printf("MAIN EXIT\n");

	return 0;
}
