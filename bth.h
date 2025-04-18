#ifndef BTH_H
#define BTH_H

#undef ENABLED_BUFFER_MAX

#define VERSION "1.2-rc4"
#define BYTES_PER_LINE 12

#ifdef _WIN32
#define vprint(...) fprintf(stderr, __VA_ARGS__)
#else
#define vprint(...) fprintf(stderr, __VA_ARGS__), fputc('\n', stderr)
#endif

#define bdebug(...) vprint("E: " __VA_ARGS__)

#ifdef ENABLED_BUFFER_MAX
#define BUFFER_MAX 4096
#define TEXT_MAX "4096"
#else
#define TEXT_MAX "No"
#endif

#endif // BTH_H
