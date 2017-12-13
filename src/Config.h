//gbRender config file

//version
#define GB_RENDER_VERSION_MAJOR 0
#define GB_RENDER_VERSION_MINOR 0

/* #undef GB_RENDER_STATIC_LIB */

#ifdef GB_RENDER_SHARED_LIB
#define GB_RENDER_CLASS class __declspec(dllexport)
#define GB_RENDER_EXPORT __declspec(dllexport)
#elif GB_RENDER_STATIC_LIB
#define GB_RENDER_CLASS class
#define GB_RENDER_EXPORT 
#else
#define GB_RENDER_CLASS class __declspec(dllimport)
#define GB_RENDER_EXPORT __declspec(dllimport)
#endif