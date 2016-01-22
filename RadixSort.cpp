#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include "Common.h"

template <typename Key, typename Data, int SortBase, int SortBits>
void RadixSortPass (const Key *InKey, const Data *InData, Key *OutKey, Data *OutData, size_t size)
{
    const size_t SortBins = 1<<SortBits;
    // Function returning value of key
    auto key = [&] (const Key &p) {return (*(uint64_t*)&p >> SortBase) % SortBins;};

    Timer t;  t.Start();
    // Histogram keys into cnt[]
    size_t cnt[SortBins] = {0};  auto p = InKey;
    for (size_t i=0; i<size; i++)
        cnt[key(InKey[i])]++;
    t.Stop();  double speed = size/(t.Elapsed()/1000);
    printf("Counting: %.3lf milliseconds = %.3lf MB/s = %.3lf MiB/s\n", t.Elapsed(), speed/1e6, speed/(1<<20));
    
    // Inclusive scan of cnt[] => compute first output position for every bin
    for (size_t i=0,sum=0; i<SortBins; i++)
    {
        auto tmp = sum + cnt[i];
        cnt[i] = sum;
        sum = tmp;
    }
        
    // Fill OutKey & OutData
    const size_t N = 16;
    Key TempKey[SortBins*N];  Data TempData[SortBins*N];
    for (size_t i=0; i<size; i++)
    {
        auto bin = key(InKey[i]);
        auto index = bin*N + cnt[bin]%N;
        TempKey [index] = InKey[i];
        TempData[index] = InData[i];
        if (cnt[bin]%N == N-1)
        {
            index -= N-1;
            auto out = cnt[bin] - (N-1);
            memcpy (OutKey +out, TempKey +index, N*sizeof(Key));
            memcpy (OutData+out, TempData+index, N*sizeof(Data));
        }
        cnt[bin]++;
    }
}
