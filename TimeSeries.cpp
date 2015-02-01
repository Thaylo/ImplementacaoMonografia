#include "TimeSeries.h"
#include <iostream>

namespace std {

static void displayOneSample(Sample &s)
{
    cout << s.evaluation << " " << s.time;
}

void storeSample(list<Sample> *samples, int evaluation, double time)
{
    Sample s = {evaluation,time};
    samples->push_back(s);
    cout << "Storing: " << evaluation << " " << time << "\n";
}

void displayStoredSamples(list<Sample> *samples)
{
    int cnt = 0;
    int size = samples->size();
    for(list<Sample>::iterator it = samples->begin(); it != samples->end(); ++it)
    {
        displayOneSample(*it);
        if( cnt < size )
        {
            cout << ", ";    
        }
    }
    cout << "\n";
}

} /* namespace std */
