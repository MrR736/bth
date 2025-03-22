#ifndef BTH_H
#define BTH_H

// #define ENABLED_BUFFER_MAX
#undef ENABLED_BUFFER_MAX

#define VERSION "1.2-rc3"
#define BYTES_PER_LINE 12

#ifdef _WIN32
#define vprintf(...) fprintf(stderr, __VA_ARGS__)
#else
#define vprintf(...) fprintf(stderr, __VA_ARGS__), fputc('\n', stderr)
#endif

#define print_error(...) vprintf("E: " __VA_ARGS__)

#ifdef ENABLED_BUFFER_MAX
#define BUFFER_MAX 4096
#define TEXT_MAX "4096"
#else
#define TEXT_MAX "No"
#endif

#endif // BTH_H
