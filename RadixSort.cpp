#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include "Common.h"

// Radix-sort key+data
template <typename Key, typename Data, int SortBase, int SortBits, bool HasData=true>
void RadixSortPass (const Key *InKey, const Data *InData, Key *OutKey, Data *OutData, size_t size)
{
    constexpr size_t SortBins = size_t(1) << SortBits,  FirstByte = SortBase/8,  FirstBit = SortBase%8;
    static_assert(FirstBit+SortBits<=sizeof(size_t)*8, "Sorting key is too wide for key() function below");

    // Function returning value of key
    auto key = [&] (const Key &p) {return (*(size_t*)(FirstByte+(char*)&p) >> FirstBit) % SortBins;};

    // Histogram keys into cnt[]
    size_t cnt[SortBins] = {0};  auto p = InKey;
    for (size_t i=0; i<size; i++)
        cnt[key(InKey[i])]++;

    // Inclusive scan of cnt[] => compute first output position for every bin
    for (size_t i=0,sum=0; i<SortBins; i++)
    {
        auto tmp = sum + cnt[i];
        cnt[i] = sum;
        sum = tmp;
    }

    // Fill OutKey & OutData
    const size_t N = CACHE_ROW/sizeof(Key);
    Key TempKey[SortBins*N];  Data TempData [HasData? SortBins*N : 1];
    for (size_t i=0; i<size; i++)
    {
        auto bin = key(InKey[i]);
        auto index = bin*N + cnt[bin]%N;
        TempKey[index] = InKey[i];
        if (HasData)
            TempData[index] = InData[i];
        if (cnt[bin]%N == N-1)
        {
            auto memcpy = [] (void* outp, void* inp, size_t size)
            {
                __m128i* out = (__m128i*) outp;
                __m128i* in  = (__m128i*) inp;
                for (size_t i=0; i<size/sizeof(__m128i); i++)
                    _mm_stream_si128(out++, *in++);
            };

            index -= N-1;
            auto out = cnt[bin] - (N-1);
            memcpy (OutKey+out, TempKey+index, N*sizeof(Key));
            if (HasData)
                memcpy (OutData+out, TempData+index, N*sizeof(Data));
        }
        cnt[bin]++;
    }
}


// Key-only sorting
template <typename Key, int SortBase, int SortBits>
void RadixSortPass (const Key *InKey, Key *OutKey, size_t size)
{
    RadixSortPass<Key,Key,0,SortBits,false> (InKey, InKey, OutKey, OutKey, size);
}


// Radix-sort key+data
template <typename Key, typename Data, bool HasData=true>
void RadixSort (const Key *InKey, const Data *InData, Key *OutKey, Data *OutData, size_t size, int FirstSortByte, int SortBytes)
{
    auto First = FirstSortByte;
    auto Last  = First+SortBytes;
    if (First<=7 && 7<Last)  RadixSortPass<Key,Data,56,8,HasData> (InKey, InData, OutKey, OutData, size);
    if (First<=6 && 6<Last)  RadixSortPass<Key,Data,48,8,HasData> (InKey, InData, OutKey, OutData, size);
    if (First<=5 && 5<Last)  RadixSortPass<Key,Data,40,8,HasData> (InKey, InData, OutKey, OutData, size);
    if (First<=4 && 4<Last)  RadixSortPass<Key,Data,32,8,HasData> (InKey, InData, OutKey, OutData, size);
    if (First<=3 && 3<Last)  RadixSortPass<Key,Data,24,8,HasData> (InKey, InData, OutKey, OutData, size);
    if (First<=2 && 2<Last)  RadixSortPass<Key,Data,16,8,HasData> (InKey, InData, OutKey, OutData, size);
    if (First<=1 && 1<Last)  RadixSortPass<Key,Data, 8,8,HasData> (InKey, InData, OutKey, OutData, size);
    if (First<=0 && 0<Last)  RadixSortPass<Key,Data, 0,8,HasData> (InKey, InData, OutKey, OutData, size);
}


// Key-only sorting
template <typename Key>
void RadixSort (const Key *InKey, Key *OutKey, size_t size, int FirstSortByte, int SortBytes)
{
    RadixSort<Key,Key,false> (InKey, InKey, OutKey, OutKey, size, FirstSortByte, SortBytes);
}
