#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include "timer.h"
#include "RadixSort.cpp"

template <typename T>
int common_bytes (T x, T y)
{
    auto t = x^y;
    for (size_t i = 0; i < sizeof(t); i++) {
        if (t%256)  return i;
        t /= 256;
    }
    return sizeof(t);
}

int main (int argc, char**argv)
{
    size_t size = uint64_t(100)<<20;
    auto buf = new char[size],   outbuf = new char[size];
    if (argc > 1) {
        auto infile = fopen(argv[1],"rb");
        size = fread(buf,1,size,infile);
    } else {
        for (size_t i=0; i<size/sizeof(uint32_t); i++)
            ((uint32_t*)buf)[i] = i*123456791;
    }
    using Key = uint64_t;  using Data = uint32_t;
    auto InKey  = new Key [size],  OutKey  = new Key [size];
    auto InData = new Data[size],  OutData = new Data[size];
    printf("Sorting %d MiB", int(size>>20));

    for (int order=0; order<=8; order++)
    {
        Timer t;  double speed;
        t.Start();
        for (size_t i = 0; i < size; i++)
            InKey[i] = *(Key*)(buf+i),  InData[i] = i;
        t.Stop();  speed = size/(t.Elapsed()/1000);
        if (!order)  printf(":  %9.3lf ms = %7.3lf MB/s = %7.3lf MiB/s\n",
                            t.Elapsed(), speed/1e6, speed/(1<<20));

        t.Start();
        //std::tie(InKey,OutKey)  =  RadixSort (InKey, OutKey, size, 0, order?order:1);
        std::tie(InKey,InData,OutKey,OutData)  =  RadixSort (InKey, InData, OutKey, OutData, size, 0, order?order:1);
        t.Stop();  speed = size/(t.Elapsed()/1000);
        if (!order)  continue;

        size_t matches = 0;
        for (size_t i = 0; i < size; i++)
        {
            if (common_bytes(InKey[i],InKey[i+1]) == order)
                matches++;
        }
        printf("%dB: %9d matches;  %9.3lf ms = %7.3lf MB/s = %7.3lf MiB/s\n",
                order, int(matches), t.Elapsed(), speed/1e6, speed/(1<<20));
    }
    return 0;
}
