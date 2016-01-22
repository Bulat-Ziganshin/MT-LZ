#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include "timer.h"
#include "RadixSort.cpp"

int main()
{
    // BENCHMARK
    const uint64_t DATASET = uint64_t(100)<<20;
    using Key = uint32_t;  using Data = uint32_t;
    auto keys = new Key [DATASET],  outkeys = new Key [DATASET];
    auto data = new Data[DATASET],  outdata = new Data[DATASET];
    
    Timer t;  double speed;

    for (size_t i=0; i<DATASET; i++)
        keys[i] = i*123456791;
    t.Start();
    RadixSortPass<Key,Data,0,4> (keys, data, outkeys, outdata, DATASET);
    t.Stop();  speed = DATASET/(t.Elapsed()/1000);
    printf("Sorting %d MiB", int(DATASET>>20));
    printf(": %.3lf milliseconds = %.3lf MB/s = %.3lf MiB/s\n\n", t.Elapsed(), speed/1e6, speed/(1<<20));

    for (size_t i=0; i<DATASET; i++)
        keys[i] = i*123456791;
    t.Start();
    RadixSortPass<Key,Data,0,6> (keys, data, outkeys, outdata, DATASET);
    t.Stop();  speed = DATASET/(t.Elapsed()/1000);
    printf("Sorting %d MiB", int(DATASET>>20));
    printf(": %.3lf milliseconds = %.3lf MB/s = %.3lf MiB/s\n\n", t.Elapsed(), speed/1e6, speed/(1<<20));

    for (size_t i=0; i<DATASET; i++)
        keys[i] = i*123456791;
    t.Start();
    RadixSortPass<Key,Data,0,8> (keys, data, outkeys, outdata, DATASET);
    t.Stop();  speed = DATASET/(t.Elapsed()/1000);
    printf("Sorting %d MiB", int(DATASET>>20));
    printf(": %.3lf milliseconds = %.3lf MB/s = %.3lf MiB/s\n\n", t.Elapsed(), speed/1e6, speed/(1<<20));

    for (size_t i=0; i<DATASET; i++)
        keys[i] = i*123456791;
    t.Start();
    RadixSortPass<Key,Data,0,12> (keys, data, outkeys, outdata, DATASET);
    t.Stop();  speed = DATASET/(t.Elapsed()/1000);
    printf("Sorting %d MiB", int(DATASET>>20));
    printf(": %.3lf milliseconds = %.3lf MB/s = %.3lf MiB/s\n\n", t.Elapsed(), speed/1e6, speed/(1<<20));

    for (size_t i=0; i<DATASET; i++)
        keys[i] = i*123456791;
    t.Start();
    RadixSortPass<Key,Data,0,16> (keys, data, outkeys, outdata, DATASET);
    t.Stop();  speed = DATASET/(t.Elapsed()/1000);
    printf("Sorting %d MiB", int(DATASET>>20));
    printf(": %.3lf milliseconds = %.3lf MB/s = %.3lf MiB/s\n\n", t.Elapsed(), speed/1e6, speed/(1<<20));

    for (size_t i=0; i<DATASET; i++)
        keys[i] = i*123456791;
    t.Start();
    RadixSortPass<Key,Data,0,20> (keys, data, outkeys, outdata, DATASET);
    t.Stop();  speed = DATASET/(t.Elapsed()/1000);
    printf("Sorting %d MiB", int(DATASET>>20));
    printf(": %.3lf milliseconds = %.3lf MB/s = %.3lf MiB/s\n\n", t.Elapsed(), speed/1e6, speed/(1<<20));

    return 0;
}
