#if __GNUC__
#include <x86intrin.h>
#define ALIGN(n)      __attribute__ ((aligned(n)))
#elif _MSC_VER
#include <intrin.h>
#define ALIGN(n)      __declspec(align(n))
#else
#define ALIGN(n)
#endif

#if (GCC_VERSION >= 304)
#  define prefetch(var)   __builtin_prefetch(&(var))
#elif _MSC_VER
#  define prefetch(var)   (_mm_prefetch((char*)&(var),_MM_HINT_T0))
#else
#  define prefetch(var)   ((void)0)
#endif

#define CACHE_ROW 64  /* size of typical CPU cache line: from long data only every 64'th byte should be prefetched */
