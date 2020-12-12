/* Macros used to define new VS API functions.

   Feb 2017, M. Sayers. Created file, contents used to be in vs_deftypes.h.
*/
#ifndef _VS_DEF_API_H
#define _VS_DEF_API_H

#if (defined(_WIN32) || defined(_WIN64)) && !defined(_LABVIEW_RT)
#define _CRTDBG_MAP_ALLOC // look for memory leaks
#include <stdlib.h>
#ifdef _MSC_VER
#include <crtdbg.h>
#endif
#define VS_API __declspec(dllexport) // declaring public function in a DLL
#define VS_API_2
#elif defined(_VS_LINUX)
#define VS_API
#define VS_API_2 __attribute__((visibility("default")))
#else
#define VS_API
#define VS_API_2
#endif

#endif
