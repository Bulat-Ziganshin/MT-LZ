#include <stdlib.h>
#include <stdint.h>
#include <memory.h>

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
    for (size_t i=0; i<size; i++)
    {
        auto bin = key(InKey[i]);
        OutKey [cnt[bin]] = InKey[i];
        OutData[cnt[bin]] = InData[i];
        cnt[bin]++;
    }
}

