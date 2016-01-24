#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include "Common.h"

template <typename Key, typename Data, int SortBase, int SortBits, bool HasData=true>
class RadixSortImplementation
{
    static constexpr size_t SortBins = size_t(1) << SortBits,  FirstByte = SortBase/8,  FirstBit = SortBase%8;
    size_t cnt[SortBins] = {0};
    // Function computing the sorting key
    size_t key (const Key &p) {return (*(size_t*)(FirstByte+(char*)&p) >> FirstBit) % SortBins;};

public:
    RadixSortPass()
    {
        static_assert (FirstBit+SortBits <= sizeof(size_t)*CHAR_BIT,
                       "Sorting key is too wide for the key() function");
    }

    void BuildHistogram (const Key *InKey, size_t size)
    {
        // Histogram keys into cnt[]
        for (size_t i=0; i<size; i++)
            cnt[key(InKey[i])]++;

        // Exclusive scan of cnt[] => compute first output position for every bin
        for (size_t bin=0,sum=0; bin<SortBins; bin++)
        {
            auto tmp = sum + cnt[bin];
            cnt[bin] = sum;
            sum = tmp;
        }
    }

    void NaiveRadixSortPass (const Key *InKey, const Data *InData, Key *OutKey, Data *OutData, size_t size)
    {
        // Fill OutKey & OutData
        for (size_t i=0; i<size; i++)
        {
            auto bin = key(InKey[i]);
            OutKey[cnt[bin]] = InKey[i];
            if (HasData)
                OutData[cnt[bin]] = InData[i];
            cnt[bin]++;
        }
    }

    void RadixSortPass (const Key *InKey, const Data *InData, Key *OutKey, Data *OutData, size_t size)
    {
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

        // Save data remainder from TempKey/TempData
        for (size_t bin=0; bin<SortBins; bin++)
        {
            auto first_index = bin*N + (bin==0 || (cnt[bin-1]/N < cnt[bin]/N)? 0 : cnt[bin-1]%N);   // first item to save
            auto last_index  = bin*N + cnt[bin]%N;   // last item to save
            auto items       = last_index - first_index;
            memcpy (OutKey+cnt[bin]-items, TempKey+first_index, items*sizeof(Key));
            if (HasData)
                memcpy (OutData+cnt[bin]-items, TempData+first_index, items*sizeof(Key));
        }
    }
};


// Radix-sort key+data
template <typename Key, typename Data, int SortBase, int SortBits, bool HasData=true>
void RadixSortPass (const Key *InKey, const Data *InData, Key *OutKey, Data *OutData, size_t size)
{
    RadixSortImplementation<Key,Data,SortBase,SortBits,HasData> impl;
    impl.BuildHistogram (InKey, size);
    impl.RadixSortPass (InKey, InData, OutKey, OutData, size);
}

// Key-only radix sort
template <typename Key, int SortBase, int SortBits>
void RadixSortPass (const Key *InKey, Key *OutKey, size_t size)
{
    RadixSortPass<Key,Key,0,SortBits,false> (InKey, InKey, OutKey, OutKey, size);
}


// Multi-pass radix-sort key+data
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


// Multi-pass key-only radix sort
template <typename Key>
void RadixSort (const Key *InKey, Key *OutKey, size_t size, int FirstSortByte, int SortBytes)
{
    RadixSort<Key,Key,false> (InKey, InKey, OutKey, OutKey, size, FirstSortByte, SortBytes);
}
