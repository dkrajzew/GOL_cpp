#ifndef config_h
#define config_h

// we are on Intel, have little-endian
#define LITTLE_ENDIAN true

#if !defined _MSC_VER && !defined __BORLANDC__
#error This file is for MSVC compilation only. GCC should use configure generated config.h.
#endif

/* Disable "identifier truncated in debug info" warnings. */
#pragma warning(disable: 4786)
/* Disable "C++ Exception Specification ignored" warnings */
#pragma warning(disable: 4290)
/* Disable "C++ Exception Specification ignored" warnings */
#pragma warning(disable: 4355)

#ifndef WIN32
#define WIN32
#endif

#ifdef _DEBUG
//#define DEBUG_ALLOC_NVWA 1
#endif

#endif
