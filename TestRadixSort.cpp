#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include "timer.h"
#include "RadixSort.cpp"

template <int Bits, typename Key, typename Data>
void BENCHMARK (Key *keys, Data *data, Key *outkeys, Data *outdata, size_t size)
{
    Timer t;  double speed;

    for (size_t i=0; i<size; i++)
        keys[i] = i*123456791;
    printf("%2d: ", Bits);
    t.Start();
//    RadixSortPass<Key,Data,0,Bits> (keys, data, outkeys, outdata, size);
    RadixSortPass<Key,0,Bits> (keys, outkeys, size);
    t.Stop();  speed = size/(t.Elapsed()/1000);
    printf("%9.3lf ms = %7.3lf MB/s = %7.3lf MiB/s : %2d\n", t.Elapsed(), speed/1e6, speed/(1<<20), Bits);
}

int main()
{
    const uint64_t size = uint64_t(100)<<20;
    using Key = uint64_t;  using Data = uint32_t;
    auto keys = new Key [size],  outkeys = new Key [size];
    auto data = new Data[size],  outdata = new Data[size];

    printf("Sorting %d MiB\n", int(size>>20));
    BENCHMARK<4>(keys, data, outkeys, outdata, size);
    BENCHMARK<5>(keys, data, outkeys, outdata, size);
    BENCHMARK<6>(keys, data, outkeys, outdata, size);
    BENCHMARK<7>(keys, data, outkeys, outdata, size);
    BENCHMARK<8>(keys, data, outkeys, outdata, size);
    BENCHMARK<9>(keys, data, outkeys, outdata, size);
    BENCHMARK<10>(keys, data, outkeys, outdata, size);
    BENCHMARK<11>(keys, data, outkeys, outdata, size);
    BENCHMARK<12>(keys, data, outkeys, outdata, size);
    BENCHMARK<13>(keys, data, outkeys, outdata, size);
    BENCHMARK<14>(keys, data, outkeys, outdata, size);
    BENCHMARK<15>(keys, data, outkeys, outdata, size);
    BENCHMARK<16>(keys, data, outkeys, outdata, size);
    BENCHMARK<17>(keys, data, outkeys, outdata, size);
    BENCHMARK<18>(keys, data, outkeys, outdata, size);
    BENCHMARK<19>(keys, data, outkeys, outdata, size);

    printf("\n");
    for (int Bytes=1; Bytes<=4; Bytes++)
    {
        Timer t;  double speed;
        for (size_t i=0; i<size; i++)
            keys[i] = i*123456791;
        printf("%1dB: ", Bytes);
        t.Start();
        RadixSort (keys, outkeys, size, 0, Bytes);
        t.Stop();  speed = size/(t.Elapsed()/1000);
        printf("%9.3lf ms = %7.3lf MB/s = %7.3lf MiB/s\n", t.Elapsed(), speed/1e6, speed/(1<<20));
    }

    delete[] keys; delete[] data; delete[] outkeys; delete[] outdata;
    return 0;
}
