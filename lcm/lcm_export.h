
#ifndef LCM_EXPORT_H
#define LCM_EXPORT_H

#ifdef LCM_STATIC
#  define LCM_EXPORT
#  define LCM_NO_EXPORT
#else
#  ifndef LCM_EXPORT
#    ifdef lcm_EXPORTS
        /* We are building this library */
#      define LCM_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define LCM_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef LCM_NO_EXPORT
#    define LCM_NO_EXPORT 
#  endif
#endif

#ifndef LCM_DEPRECATED
#  define LCM_DEPRECATED __declspec(deprecated)
#endif

#ifndef LCM_DEPRECATED_EXPORT
#  define LCM_DEPRECATED_EXPORT LCM_EXPORT LCM_DEPRECATED
#endif

#ifndef LCM_DEPRECATED_NO_EXPORT
#  define LCM_DEPRECATED_NO_EXPORT LCM_NO_EXPORT LCM_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef LCM_NO_DEPRECATED
#    define LCM_NO_DEPRECATED
#  endif
#endif

#endif /* LCM_EXPORT_H */
