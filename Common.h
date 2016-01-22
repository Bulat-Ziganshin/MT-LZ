#if (GCC_VERSION >= 304)
#  define prefetch(var)   __builtin_prefetch(&(var))
#elif 1
#include <xmmintrin.h>
#  define prefetch(var)   (_mm_prefetch((char*)&(var),_MM_HINT_T0))
#else
#  define prefetch(var)   ((void)0)
#endif

#define CACHE_ROW 64  /* size of typical CPU cache line: from long data only every 64'th byte should be prefetched */

