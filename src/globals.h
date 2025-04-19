#ifndef GLOBALS
#define GLOBALS

#define False 0
#define True 1

#define FILENAME argv[1]
#define FILENAME_EXT {FILENAME[filename_len-4], FILENAME[filename_len-3], FILENAME[filename_len-2], FILENAME[filename_len-1], '\0'}
#define MIN_FILENAME_LEN 5
#define EXENAME argv[2]

typedef char Bool;

#endif
