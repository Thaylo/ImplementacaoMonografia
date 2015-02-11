#include "BateriaDeTestes.h"
#include "TimeSeries.h"
#include "CommonDebug.h"
#include "AsyncIO.h"

#define ERR 0

using namespace std;

int main()
{
    char operationMode[30];
    int rc = ERR;
    FILE *fp = fopen("config.ini","r");

    rc = fscanf(fp,"%s",operationMode);
    fclose(fp);

    if ( ERR == rc )
    {
        printf("CONFIG READ FAILURE\n");
        return 0;
    }

	BateriaDeTestes b;
	
    srand(time(NULL));
	
    if(!strcmp(operationMode,"SMALL_SIZE"))
    {
        b.run(SMALL_SIZE);    
    }
    else if(!strcmp(operationMode,"LARGE_SIZE"))
    {
        b.run(LARGE_SIZE);  
    }
    else
    {
        printf("CONFIG VALUE FAILURE\n");
        return 0;
    }


    printf("MAIN EXIT\n");
	return 0;
}
