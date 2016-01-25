#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include "timer.h"
#include "RadixSort.cpp"

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
    using Key = uint64_t;
    auto InKey = new Key[size],  OutKey = new Key[size];
    printf("Sorting %d MiB\n", int(size>>20));

    for (int Bytes=0; Bytes<=6; Bytes++)
    {
        Timer t;  double speed;
        printf("%dB: ", Bytes);
        t.Start();
        SortingTransform (buf, outbuf, size, Bytes?Bytes:1, InKey, OutKey);
        t.Stop();  speed = size/(t.Elapsed()/1000);
        printf("%9.3lf ms = %7.3lf MB/s = %7.3lf MiB/s\n", t.Elapsed(), speed/1e6, speed/(1<<20));
    }
    return 0;
}
