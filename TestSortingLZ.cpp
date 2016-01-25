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
    printf("Checking %d MiB", int(size>>20));
    if (argc > 1)
        printf(" of %s", argv[1]);

    // Warm up the data
    RadixSort (InKey, InData, OutKey, OutData, size, 0, 1);

    for (int order=1; order<=8; order++)
    {
        Timer t;  double speed;
        t.Start();
        for (size_t i = 0; i < size; i++)
            InKey[i] = *(Key*)(buf+i),  InData[i] = i;
        t.Stop();  speed = size/(t.Elapsed()/1000);
        if (order==1)  printf(":  %9.3lf ms = %7.3lf MB/s = %7.3lf MiB/s\n",
                              t.Elapsed(), speed/1e6, speed/(1<<20));

        t.Start();
        //std::tie(InKey,OutKey)  =  RadixSort (InKey, OutKey, size, 0, order);
        std::tie(InKey,InData,OutKey,OutData)  =  RadixSort (InKey, InData, OutKey, OutData, size, 0, order);
        t.Stop();  speed = size/(t.Elapsed()/1000);

        size_t matches = 0,  bins = 0,  max_bin = 0,  start = 0,  total_matches = 0,  cmps = 0,
               DEPTH = 32;
        for (size_t i = 0; i < size; i++)
        {
            auto cb = common_bytes(InKey[i], InKey[i+1]);
            if (cb == order)  matches++;
            if (cb >= order)  total_matches++;
            if (cb <  order)
            {
                bins++;
                if (i-start > max_bin)  max_bin = i-start;
                start = i;
            }
            cmps += std::min(i-start, DEPTH);
        }
        printf("%dB: %9d matches, %9d total, %10.0lf cmps; %9d bins, %9d maxbin;  ",
                order, int(matches), int(total_matches), double(cmps), int(bins), int(max_bin));
        printf("%9.3lf ms = %7.3lf MB/s = %7.3lf MiB/s\n",
                t.Elapsed(), speed/1e6, speed/(1<<20));
    }
    return 0;
}
