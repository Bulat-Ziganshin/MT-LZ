#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include "timer.h"
#include "RadixSort.cpp"

const uint64_t DATASET = uint64_t(100)<<20;

template <int Bits>
void BENCHMARK()
{
    using Key = uint32_t;  using Data = uint32_t;
    auto keys = new Key [DATASET],  outkeys = new Key [DATASET];
    auto data = new Data[DATASET],  outdata = new Data[DATASET];
    
    Timer t;  double speed;

    for (size_t i=0; i<DATASET; i++)
        keys[i] = i*123456791;
    printf("%2d: ", Bits, int(DATASET>>20));
    t.Start();
    RadixSortPass<Key,Data,0,Bits> (keys, data, outkeys, outdata, DATASET);
    t.Stop();  speed = DATASET/(t.Elapsed()/1000);
    printf(" ms : %9.3lf ms = %7.3lf MB/s = %7.3lf MiB/s : %2d\n", t.Elapsed(), speed/1e6, speed/(1<<20), Bits);
    delete[] keys; delete[] data; delete[] outkeys; delete[] outdata;
}

int main()
{
    printf("Sorting %d MiB\n", int(DATASET>>20));
    BENCHMARK<4>();
    BENCHMARK<5>();
    BENCHMARK<6>();
    BENCHMARK<7>();
    BENCHMARK<8>();
    BENCHMARK<9>();
    BENCHMARK<10>();
    BENCHMARK<11>();
    BENCHMARK<12>();
    BENCHMARK<13>();
    BENCHMARK<14>();
    BENCHMARK<15>();
    BENCHMARK<16>();
    BENCHMARK<17>();
    BENCHMARK<18>();
    BENCHMARK<19>();
    return 0;
}
