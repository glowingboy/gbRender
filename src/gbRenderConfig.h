//gbRender config file

//version
#define GB_RENDER_VERSION_MAJOR 0
#define GB_RENDER_VERSION_MINOR 0

#ifdef GB_RENDER_SHARED_LIB
#define GB_RENDER_CLASS class __declspec(dllexport)
#elif GB_RENDER_STATIC_LIB
#define GB_RENDER_CLASS class
#else
#define GB_RENDER_CLASS class __declspec(dllimport)
#endif
