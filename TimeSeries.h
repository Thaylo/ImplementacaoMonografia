#ifndef _TIME_SERIES_H
#define _TIME_SERIES_H

#include <list>

namespace std {

typedef struct
{
    double evaluation;
    double time;
} Sample;


void storeSample(list<Sample> *samples, double evaluation, double time);

void displayStoredSamples(list<Sample> *samples);

} /* namespace std */

#endif /* _TIME_SERIES_H */
