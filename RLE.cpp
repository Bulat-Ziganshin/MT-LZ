#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h>
#include <memory.h>

int main (int argc, char**argv)
{
    size_t size = uint64_t(1000)<<20;
    auto buf = new char[size],   outbuf = new char[size];
    if (argc != 4) {
        printf("Usage: RLE n infile outfile\n");
        return 0;
    }
    auto N = atoi(argv[1]);
    auto infile = fopen(argv[2],"rb");
    size = fread(buf,1,size,infile);
    auto outfile = fopen(argv[3],"wb");

    int c = -1, run=0;
    auto out = outbuf;
    for (size_t i = 0; i < size; i++)
    {
        buf[i]==c? run++ : run=1;
        if (run==N)
            out -= 15;
        else if (run<N)
            c = *out++ = buf[i];
    }

    fwrite(outbuf,1,out-outbuf,outfile);
    return 0;
}
