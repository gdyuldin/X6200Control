
#ifndef AETHER_X6200CTRL_API_H
#define AETHER_X6200CTRL_API_H

#ifdef AETHER_X6200CTRL_STATIC_DEFINE
#  define AETHER_X6200CTRL_API
#  define AETHER_X6200CTRL_NO_EXPORT
#else
#  ifndef AETHER_X6200CTRL_API
#    ifdef AETHER_X6200CTRL_EXPORTING
        /* We are building this library */
#      define AETHER_X6200CTRL_API __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define AETHER_X6200CTRL_API __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef AETHER_X6200CTRL_NO_EXPORT
#    define AETHER_X6200CTRL_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef AETHER_X6200CTRL_DEPRECATED
#  define AETHER_X6200CTRL_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef AETHER_X6200CTRL_DEPRECATED_EXPORT
#  define AETHER_X6200CTRL_DEPRECATED_EXPORT AETHER_X6200CTRL_API AETHER_X6200CTRL_DEPRECATED
#endif

#ifndef AETHER_X6200CTRL_DEPRECATED_NO_EXPORT
#  define AETHER_X6200CTRL_DEPRECATED_NO_EXPORT AETHER_X6200CTRL_NO_EXPORT AETHER_X6200CTRL_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef AETHER_X6200CTRL_NO_DEPRECATED
#    define AETHER_X6200CTRL_NO_DEPRECATED
#  endif
#endif

#endif /* AETHER_X6200CTRL_API_H */
