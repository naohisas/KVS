/****************************************************************************/
/**
 *  @file Timer.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Timer.h 1707 2014-01-27 07:37:04Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS_CORE_TIMER_H_INCLUDE
#define KVS_CORE_TIMER_H_INCLUDE

#include <ctime>
#include <iostream>
#include <vector>
#include <kvs/Compiler>
#include <kvs/Platform>
#if defined ( KVS_COMPILER_VC )
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#else // KVS_COMPILER_GCC
#include <sys/time.h>
#include <sys/timeb.h>
#endif
#include <kvs/Type>
#include <kvs/Message>


/* Hight precision CPU counter using RDTCS or QPC. It becomes possible to use
 * the CPU counter by defining 'KVS_TIMER_USE_RDTSC' or 'KVS_TIMER_USE_QPC'.
 * In the default setting, Timer function is implemented by using gettimeofday()
 * without the CPU counter.
 *
 * #define KVS_TIMER_ENABLE_RDTSC // ReaD Time Stamp Counter
 * #define KVS_TIMER_ENABLE_QPC   // Query Performance Counter
 */
#if defined( KVS_TIMER_ENABLE_RDTSC ) || defined( KVS_TIMER_ENABLE_QPC )
#define KVS_TIMER_USE_CPU_COUNTER
#else
#define KVS_TIMER_USE_GETTIMEOFDAY
#endif

/*==========================================================================*/
/*
 *  RDTSC: ReaD Time Stamp Counter (Intel Pentium CPU only)
 */
/*==========================================================================*/
#if defined ( KVS_TIMER_ENABLE_RDTSC )
#if defined ( KVS_COMPILER_VC )
#define KVS_TIMER_GET_RDTSC( counter )           \
    {                                            \
        __asm {                                  \
            __asm _emit 0x0f                     \
            __asm _emit 0x31                     \
            __asm mov dword ptr [counter],   eax \
            __asm mov dword ptr [counter+4], edx \
        }                                        \
    }

#elif defined ( KVS_COMPILER_GCC )
#if defined ( KVS_PLATFORM_CPU_X86 ) || defined ( KVS_PLATFORM_CPU_I386 )
#define KVS_TIMER_GET_RDTSC( counter )                                \
    {                                                                 \
        __asm__ volatile (".byte 0x0f, 0x31" : "=A" (counter.value)); \
    }

#elif defined ( KVS_PLATFORM_CPU_POWERPC )
#define KVS_TIMER_GET_RDTSC( counter )                                         \
    {                                                                          \
        uint32_t tmp;                                                          \
        __asm__ volatile ( "loop:                  \n"                         \
                           "\tmftbu   %0           \n"                         \
                           "\tmftb    %1           \n"                         \
                           "\tmftbu   %2           \n"                         \
                           "\tcmpw    %2,%0        \n"                         \
                           "\tbne     loop         \n"                         \
                           : "=r"(counter.high),"=r"(counter.low),"=r"(tmp) ); \
    }

// Not supported.
#else
#error Not supported RDTSC.
#endif

// Unknown compiler.
#else
#error Unknown compiler. KVS supports GNU C++ compiler and
#error Microsoft Visual C++ compiler only.
#endif
#endif
// end of KVS_TIMER_ENABLE_RDTSC for KVS_TIMER_GET_RDTSC()

/*==========================================================================*/
/*
 *  QPC: Query Performance Counter (Microsoft Visual C++ only)
 */
/*==========================================================================*/
#if defined ( KVS_TIMER_ENABLE_QPC )
#if defined ( KVS_COMPILER_VC )
#define KVS_TIMER_GET_QPC( counter )                                           \
    {                                                                          \
        QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*>( &counter ) ); \
    }

// Not supported.
#else
#error Not supported QPC.
#endif
#endif
// end of KVS_TIMER_ENABLE_QPC for KVS_TIMER_GET_QPC()


namespace kvs
{

/*==========================================================================*/
/**
 *  Time-stamp (64 bit integer value)
 */
/*==========================================================================*/
#if defined ( KVS_TIMER_USE_CPU_COUNTER )
union TimeStamp
{
    union
    {
        struct
        {
            kvs::UInt32 low;  ///< low bit (32 bit)
            kvs::UInt32 high; ///< high bit (32 bit)
        };
        kvs::UInt64 value;    ///< 64 bit integer value
    };
};
#else
typedef timeval TimeStamp;
#endif

#if defined ( KVS_TIMER_USE_CPU_COUNTER )
/*==========================================================================*/
/**
 *  Get CPU counter.
 *  @return CPU clocks
 */
/*==========================================================================*/
inline TimeStamp GetCPUCounter()
{
    TimeStamp counter;

#if defined ( KVS_TIMER_ENABLE_RDTSC )
    KVS_TIMER_GET_RDTSC( counter );
#else // KVS_TIMER_ENABLE_QPC
    KVS_TIMER_GET_QPC( counter );
#endif

    return counter;
}

#else // KVS_TIMER_USE_GETTIMEOFDAY
/*==========================================================================*/
/**
 *  OS independent gettimeofday function.
 *  @param tv [in] time value
 */
/*==========================================================================*/
inline void GetTimeOfDay( struct timeval& tv )
{
    /* In the case of MinGW as follows:
     *
     * struct _timeb tb; _ftime( &tb );
     * tv.tv_sec  = tb.time;
     * tv.tv_usec = tb.millitm * 1000 + 500;
     */
#if defined ( KVS_COMPILER_VC )
    LARGE_INTEGER time = {0};
    if ( QueryPerformanceCounter( &time ) )
    {
        LARGE_INTEGER freq = {0};
        QueryPerformanceFrequency( &freq );
        tv.tv_sec  = static_cast<long>( time.QuadPart / freq.QuadPart );
        tv.tv_usec = static_cast<long>( ( static_cast<float>( time.QuadPart ) / freq.QuadPart - tv.tv_sec ) * 1000000 );
    }
    else
    {
        DWORD t = timeGetTime();
        tv.tv_sec  = t / 1000;
        tv.tv_usec = t % 1000;
    }
/*
#if defined ( KVS_PLATFORM_CPU_64 )
    LARGE_INTEGER freq = {0};
    LARGE_INTEGER time = {0};
    QueryPerformanceFrequency( &freq );
    QueryPerformanceCounter( &time );
    tv.tv_sec  = 0L;
    tv.tv_usec = time.QuadPart * 1000L / freq.QuadPart;
#else
    tv.tv_sec  = 0L;
    tv.tv_usec = timeGetTime() * 1000L;
#endif
*/
#else // KVS_COMPILER_GCC
    gettimeofday( &tv, NULL );
#endif
}
#endif

/*==========================================================================*/
/**
 *  Get CPU fequency.
 *  @return CPU frequency [Hz] = [clock/sec]
 */
/*==========================================================================*/
inline kvs::UInt32 GetCPUFrequency()
{
    kvs::UInt32 frequency = 0;

#if defined ( KVS_TIMER_ENABLE_RDTSC )
    TimeStamp start_tsc = GetCPUCounter();
    // Sleep 1 sec.
#if defined ( KVS_COMPILER_VC )
    Sleep( 1000 );
#else // KVS_COMPILER_GCC
    sleep( 1 );
#endif
    TimeStamp end_tsc = GetCPUCounter();
    frequency = end_tsc.value - start_tsc.value;
#elif defined ( KVS_TIMER_ENABLE_QPC )
    SetPriorityClass( GetCurrentProcess(), REALTIME_PRIORITY_CLASS );
    SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL );
    QueryPerformanceFrequency( reinterpret_cast<LARGE_INTEGER*>( &frequency ) );
#else
    frequency = CLOCKS_PER_SEC;
#endif

    return frequency;
}

// Weight values for converting time unit.
#if defined ( KVS_TIMER_USE_CPU_COUNTER )
const kvs::UInt32 KVS_TIMER_CPU_HZ        = GetCPUFrequency();
const kvs::Real64 KVS_TIMER_CLOCK_TO_SEC  = 1.0 / KVS_TIMER_CPU_HZ;
const kvs::Real64 KVS_TIMER_CLOCK_TO_MSEC = KVS_TIMER_CLOCK_TO_SEC  * 1000.0;
const kvs::Real64 KVS_TIMER_CLOCK_TO_USEC = KVS_TIMER_CLOCK_TO_MSEC * 1000.0;
#endif
const kvs::Real64 KVS_TIMER_SEC_TO_USEC   = 1000000.0;
const kvs::Real64 KVS_TIMER_SEC_TO_MSEC   =    1000.0;
const kvs::Real64 KVS_TIMER_USEC_TO_SEC   =  0.000001;
const kvs::Real64 KVS_TIMER_USEC_TO_MSEC  =     0.001;

/*==========================================================================*/
/**
 *  Get current time stamp.
 *  @return time-stamp
 */
/*==========================================================================*/
inline TimeStamp GetStamp()
{
#if defined ( KVS_TIMER_USE_CPU_COUNTER )
    return GetCPUCounter();
#else
    TimeStamp ts;
    GetTimeOfDay( ts );
    return ts;
#endif
}

/*==========================================================================*/
/**
 *  Convert time-stamp to time [sec].
 *  @param ts [in] time-stamp
 *  @return time [sec]
 */
/*==========================================================================*/
inline double TimeStampToSec( const TimeStamp& ts )
{
#if defined ( KVS_TIMER_USE_CPU_COUNTER )
    return ts.value * KVS_TIMER_CLOCK_TO_SEC;
#else
    return ts.tv_sec + ts.tv_usec * KVS_TIMER_USEC_TO_SEC;
#endif
}

/*==========================================================================*/
/**
 *  Convert time-stamp to time [msec].
 *  @param ts [in] time-stamp
 *  @return time [msec]
 */
/*==========================================================================*/
inline double TimeStampToMSec( const TimeStamp& ts )
{
#if defined ( KVS_TIMER_USE_CPU_COUNTER )
    return ts.value * KVS_TIMER_CLOCK_TO_MSEC;
#else
    return ts.tv_sec  * KVS_TIMER_SEC_TO_MSEC +
           ts.tv_usec * KVS_TIMER_USEC_TO_MSEC;
#endif
}

/*==========================================================================*/
/**
 *  Convert time-stamp to time [usec].
 *  @param ts [in] time-stamp
 *  @return time [usec]
 */
/*==========================================================================*/
inline double TimeStampToUSec( const TimeStamp& ts )
{
#if defined ( KVS_TIMER_USE_CPU_COUNTER )
    return ts.value * KVS_TIMER_CLOCK_TO_USEC;
#else
    return ts.tv_sec * KVS_TIMER_SEC_TO_USEC + ts.tv_usec;
#endif
}

/*==========================================================================*/
/**
 *  Get difference time-stamp.
 *  @param ts1 [in] time-stamp #1
 *  @param ts2 [in] time-stamp #2
 *  @return time-stamp
 */
/*==========================================================================*/
inline TimeStamp TimeStampDiff( const TimeStamp& ts1, const TimeStamp& ts2 )
{
    TimeStamp ts;
#if defined ( KVS_TIMER_USE_CPU_COUNTER )
    ts.low  = ts1.low - ts2.low;
    ts.high = ts.low > ts1.low ? ts1.high - ts2.high - 1 : ts1.high - ts2.high;
#else
    ts.tv_sec  = ts1.tv_sec  - ts2.tv_sec;
    ts.tv_usec = ts1.tv_usec - ts2.tv_usec;
#endif
    return ts;
}

/*==========================================================================*/
/**
 *  Get difference time-stamp in second.
 *  @param ts1 [in] time-stamp #1
 *  @param ts2 [in] time-stamp #2
 *  @return time-stamp [sec]
 */
/*==========================================================================*/
inline double TimeStampDiffInSec( const TimeStamp& ts1, const TimeStamp& ts2 )
{
    return TimeStampToSec( TimeStampDiff( ts1, ts2 ) );
}

/*==========================================================================*/
/**
 *  Get difference time-stamp in mili-second.
 *  @param ts1 [in] time-stamp #1
 *  @param ts2 [in] time-stamp #2
 *  @return time-stamp [msec]
 */
/*==========================================================================*/
inline double TimeStampDiffInMSec( const TimeStamp& ts1, const TimeStamp& ts2 )
{
    return TimeStampToMSec( TimeStampDiff( ts1, ts2 ) );
}

/*==========================================================================*/
/**
 *  Get difference time-stamp in micro-second.
 *  @param ts1 [in] time-stamp #1
 *  @param ts2 [in] time-stamp #2
 *  @return time-stamp [usec]
 */
/*==========================================================================*/
inline double TimeStampDiffInUSec( const TimeStamp& ts1, const TimeStamp& ts2 )
{
    return TimeStampToUSec( TimeStampDiff( ts1, ts2 ) );
}

/*==========================================================================*/
/**
 *  Time measurment class.
 */
/*==========================================================================*/
class Timer
{
public:
    enum Trigger { Start };

private:
    TimeStamp m_start; ///< start time
    TimeStamp m_stop;  ///< stop time

public:
    Timer();
    Timer( const Trigger trigger );
    ~Timer();

public:
    void start();
    void stop();

    double sec() const;
    double msec() const;
    double usec() const;

    double fps() const;
};

/*==========================================================================*/
/**
 *  Constructor.
 */
/*==========================================================================*/
inline Timer::Timer()
{
#if defined ( KVS_TIMER_USE_CPU_COUNTER )
    m_start.value = 0;
    m_stop.value  = 0;
#else
    m_start.tv_sec  = 0;
    m_start.tv_usec = 0;
    m_stop.tv_sec   = 0;
    m_stop.tv_usec  = 0;
#endif
}

/*==========================================================================*/
/**
 *  Constructor.
 *  @param trigger [in] trigger ( 'kvs::Timer::Start' is given )
 */
/*==========================================================================*/
inline Timer::Timer( Trigger trigger )
{
    if ( trigger != Timer::Start )
    {
        kvsMessageError( "Unknown trigger." );
        return;
    }
    this->start();
}

/*==========================================================================*/
/**
 *  Destructor.
 */
/*==========================================================================*/
inline Timer::~Timer()
{
}

/*==========================================================================*/
/**
 *  Strat timer.
 */
/*==========================================================================*/
inline void Timer::start()
{
    m_start = GetStamp();
}

/*==========================================================================*/
/**
 *  Stop timer.
 */
/*==========================================================================*/
inline void Timer::stop()
{
    m_stop = GetStamp();
}

/*==========================================================================*/
/**
 *  Get elapse time.
 *  @return elapse time [sec]
 */
/*==========================================================================*/
inline double Timer::sec() const
{
    return TimeStampDiffInSec( m_stop, m_start );
}

/*==========================================================================*/
/**
 *  Get elapse time.
 *  @return elapse time [msec]
 */
/*==========================================================================*/
inline double Timer::msec() const
{
    return TimeStampDiffInMSec( m_stop, m_start );
}

/*==========================================================================*/
/**
 *  Get elapse time.
 *  @return elapse time [usec]
 */
/*==========================================================================*/
inline double Timer::usec() const
{
    return TimeStampDiffInUSec( m_stop, m_start );
}

/*==========================================================================*/
/**
 *  Get frame rate.
 *  @return frame rate [fps]
 */
/*==========================================================================*/
inline double Timer::fps() const
{
    return 1.0 / TimeStampDiffInSec( m_stop, m_start );
}

} // end of namespace kvs

#endif // KVS_CORE_TIMER_H_INCLUDE
