#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include "timer.h"
#include "RadixSort.cpp"

int main (int argc, char**argv)
{
    if (argc <= 2) {
        printf("Usage: SortingLZ order file\n");
        return 1;
    }
    auto order = atoi(argv[1]);

    size_t size = uint64_t(100)<<20;
    auto buf = new char[size];
    auto infile = fopen(argv[2],"rb");
    size = fread(buf,1,size,infile);

    using Key = uint64_t;
    auto InKey  = new Key [size],  OutKey  = new Key [size];
    printf("Order-%d sorting LZ: %d MiB of %s\n", order, int(size>>20), argv[2]);

    Timer t;  double speed;
    t.Start();
    uint64_t mask = (order==8? 0 : uint64_t(1)<<(order*8)) - 1,  high_4_bytes = uint64_t(-1) << 32;
    for (size_t i = 0; i < size; i++)
    {
        uint64_t hash = order<=4? *(uint64_t*)(buf+i) << 32
                                : (((*(uint64_t*)(buf+i)) & mask)*123456791) & high_4_bytes;
        InKey[i] = hash | i;  // pack hash + position
    }
    t.Stop();  speed = size/(t.Elapsed()/1000);
    printf("Fill:  %9.3lf ms = %7.3lf MB/s = %7.3lf MiB/s\n", t.Elapsed(), speed/1e6, speed/(1<<20));

    t.Start();
    std::tie(InKey,OutKey)  =  RadixSort (InKey, OutKey, size, 4, std::min(order,4));
    t.Stop();  speed = size/(t.Elapsed()/1000);
    printf("Sort:  %9.3lf ms = %7.3lf MB/s = %7.3lf MiB/s\n", t.Elapsed(), speed/1e6, speed/(1<<20));

    t.Start();
    int sum = 0;
    for (size_t i = 0; i < size; i++)
    {
        sum += buf[uint32_t(InKey[i])];
    }
    t.Stop();  speed = size/(t.Elapsed()/1000) + (sum==42?1e-9:0);
    printf("Scan:  %9.3lf ms = %7.3lf MB/s = %7.3lf MiB/s\n", t.Elapsed(), speed/1e6, speed/(1<<20));

    return 0;
}
