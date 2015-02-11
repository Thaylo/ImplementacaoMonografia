#ifndef ASYNCIO_H
#define ASYNCIO_H

#include <stdio.h>
#include <thread>
#include <mutex>

using namespace std;

class AsyncChannel
{
public:
    AsyncChannel( )
    {
        fileDescriptor = NULL;
    }

    AsyncChannel( const char *filename )
    {
        fileDescriptor = fopen( filename, "w");
    }

    void read( void *data, void (*readFunc)(FILE *, void *) )
    {
        if (NULL == data) return;
        std::unique_lock<std::mutex> mlock(mutex_);
        if ( NULL != readFunc)
        {
            readFunc(fileDescriptor, data);
        }
        else
        {
            fscanf(fileDescriptor, "%s", (char*) data);
        }
    }

    void write( void * data, void (*writeFunc)(FILE *, void *) )
    {
        if (NULL == data) return;
        std::unique_lock<std::mutex> mlock(mutex_);
        if ( NULL != writeFunc)
        {
            writeFunc(fileDescriptor, (char*) data);
        }
        else
        {
            fprintf(fileDescriptor, "%s", (char*) data);
        }
    }

    ~AsyncChannel()
    {
        if( NULL != fileDescriptor)
        {
            fclose(fileDescriptor);
        }

    }

private:
    FILE *fileDescriptor;
    mutex mutex_;
};

#endif /* ASYNCIO_H */