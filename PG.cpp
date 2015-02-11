#include "BateriaDeTestes.h"
#include "TimeSeries.h"
#include "CommonDebug.h"
#include "AsyncIO.h"

using namespace std;

double estimatedTimeToAcomplish2000iterationsWith40Resamples(char *operationMode)
{
    BateriaDeTestes b;
    
    clock_t start_time = clock();    
    clock_t end_time;
    srand(time(NULL));
    
    if(!strcmp(operationMode,"SMALL_SIZE"))
    {
        b.run(SMALL_SIZE,1,1);    
        end_time = clock(); 
    }
    else if(!strcmp(operationMode,"LARGE_SIZE"))
    {
        b.run(LARGE_SIZE,1,1); 
        end_time = clock(); 
    }
    else
    {
        printf("CONFIG VALUE FAILURE\n");
        return 0;
    }
    double delta = (end_time - start_time) / (double)CLOCKS_PER_SEC;
    return delta * 2000 * 4 / 3600.0;
}

static void runTests(char *operationMode)
{
    BateriaDeTestes b;
    int mode = SMALL_SIZE;

    if(!strcmp(operationMode,"LARGE_SIZE"))
    {
        mode = LARGE_SIZE;
    }

    b.run(mode, 4,2000);
}

int main()
{
    srand(time(NULL));
    double t = estimatedTimeToAcomplish2000iterationsWith40Resamples((char*)"SMALL_SIZE");
    printf("Estimated time for small instances: %lf hours\n",t);

    t = estimatedTimeToAcomplish2000iterationsWith40Resamples((char*)"LARGE_SIZE");
    printf("Estimated time for large instances: %lf hours\n",t);

    runTests((char*)"SMALL_SIZE");
    runTests((char*)"LARGE_SIZE");
	
    return 0;
}
