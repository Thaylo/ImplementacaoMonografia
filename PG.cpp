#include "BateriaDeTestes.h"
#include "TimeSeries.h"
#include "CommonDebug.h"
#include "AsyncIO.h"

const int repetitions = 4;
const int iterations = 500;

using namespace std;

#if 0
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
#endif

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
    cout << "O sistema permite " << std::thread::hardware_concurrency() << " threads\n";
    runTests((char*)"SMALL_SIZE");
    runTests((char*)"LARGE_SIZE");
	
    return 0;
}
