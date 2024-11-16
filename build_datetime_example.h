#pragma once

// Requires umba-date utility available in PATH (see https://github.com/al-martyn1/umba-date)

// https://en.cppreference.com/w/cpp/preprocessor/include

#if defined(__has_include) && __has_include("umba_date__.h")
    #include "umba_date__.h"
    #define BUILD_DATE  UMBA_DATE__
#else
    #define BUILD_DATE  __DATE__
#endif

#if defined(__has_include) && __has_include("umba_time__.h")
    #include "umba_time__.h"
    #define BUILD_TIME  UMBA_TIME__
#else
    #define BUILD_TIME  __TIME__
#endif

