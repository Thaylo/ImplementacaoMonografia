#include "BateriaDeTestes.h"
#include "TimeSeries.h"
#include "CommonDebug.h"
#include "AsyncIO.h"

const int repetitions = 20;
const int iterations = 2000;

using namespace std;

double estimatedTimeToAcomplish(char *operationMode, int rep, int iter)
{
    BateriaDeTestes b;
    
    clock_t start_time = clock();    
    clock_t end_time;
    srand(time(NULL));
    end_time = start_time;

    if(!strcmp(operationMode,"SMALL_SIZE"))
    {
        b.run(SMALL_SIZE, rep, iter);    
        end_time = clock(); 
    }
    else if(!strcmp(operationMode,"LARGE_SIZE"))
    {
        b.run(LARGE_SIZE, rep, iter); 
        end_time = clock(); 
    }
    else
    {
        printf("CONFIG VALUE FAILURE\n");
        return 0;
    }
    double delta = (end_time - start_time) / (double) CLOCKS_PER_SEC;
    return delta * (iterations*repetitions/(rep*iter)) / 3600.0;
}

static void runTests(char *operationMode)
{
    BateriaDeTestes b;
    int mode = SMALL_SIZE;

    if(!strcmp(operationMode,"LARGE_SIZE"))
    {
        mode = LARGE_SIZE;
    }
    
    b.run(mode, repetitions, iterations);
}

int main()
{
    srand(time(NULL));
    int iter = 4;
    int rep = 4;
    double t = estimatedTimeToAcomplish((char*)"SMALL_SIZE", iter, rep);
    printf("Estimated time for small instances: %lf hours\n",t);

    t = estimatedTimeToAcomplish((char*)"LARGE_SIZE", iter, rep);
    printf("Estimated time for large instances: %lf hours\n",t);
    
    runTests((char*)"SMALL_SIZE");
    runTests((char*)"LARGE_SIZE");
	
    return 0;
}
