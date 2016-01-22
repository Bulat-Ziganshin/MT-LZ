#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include "timer.h"
#include "RadixSort.cpp"

const uint64_t DATASET = uint64_t(100)<<20;
using Key = uint32_t;  using Data = uint32_t;

template <int Bits>
void BENCHMARK (Key *keys, Data *data, Key *outkeys, Data *outdata)
{
    Timer t;  double speed;

    for (size_t i=0; i<DATASET; i++)
        keys[i] = i*123456791;
    printf("%2d: ", Bits, int(DATASET>>20));
    t.Start();
    RadixSortPass<Key,Data,0,Bits> (keys, data, outkeys, outdata, DATASET);
    t.Stop();  speed = DATASET/(t.Elapsed()/1000);
    printf(" ms : %9.3lf ms = %7.3lf MB/s = %7.3lf MiB/s : %2d\n", t.Elapsed(), speed/1e6, speed/(1<<20), Bits);
}

int main()
{
    auto keys = new Key [DATASET],  outkeys = new Key [DATASET];
    auto data = new Data[DATASET],  outdata = new Data[DATASET];
    printf("Sorting %d MiB\n", int(DATASET>>20));

    BENCHMARK<4>(keys, data, outkeys, outdata);
    BENCHMARK<5>(keys, data, outkeys, outdata);
    BENCHMARK<6>(keys, data, outkeys, outdata);
    BENCHMARK<7>(keys, data, outkeys, outdata);
    BENCHMARK<8>(keys, data, outkeys, outdata);
    BENCHMARK<9>(keys, data, outkeys, outdata);
    BENCHMARK<10>(keys, data, outkeys, outdata);
    BENCHMARK<11>(keys, data, outkeys, outdata);
    BENCHMARK<12>(keys, data, outkeys, outdata);
    BENCHMARK<13>(keys, data, outkeys, outdata);
    BENCHMARK<14>(keys, data, outkeys, outdata);
    BENCHMARK<15>(keys, data, outkeys, outdata);
    BENCHMARK<16>(keys, data, outkeys, outdata);
    BENCHMARK<17>(keys, data, outkeys, outdata);
    BENCHMARK<18>(keys, data, outkeys, outdata);
    BENCHMARK<19>(keys, data, outkeys, outdata);
    delete[] keys; delete[] data; delete[] outkeys; delete[] outdata;
    return 0;
}
