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
    for (size_t i = 0; i < size; i++)
        InKey[i] = *(uint32_t*)(buf+i) + (uint64_t(i)<<32);  // pack 4 bytes + position
    t.Stop();  speed = size/(t.Elapsed()/1000);
    printf("Fill:  %9.3lf ms = %7.3lf MB/s = %7.3lf MiB/s\n", t.Elapsed(), speed/1e6, speed/(1<<20));

    t.Start();
    std::tie(InKey,OutKey)  =  RadixSort (InKey, OutKey, size, 0, order);
    t.Stop();  speed = size/(t.Elapsed()/1000);
    printf("Sort:  %9.3lf ms = %7.3lf MB/s = %7.3lf MiB/s\n", t.Elapsed(), speed/1e6, speed/(1<<20));

    return 0;
}
