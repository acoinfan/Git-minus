#ifndef MACROS_INCLUDE_H_
#define MACROS_INCLUDE_H_

#include "gitm.h"

#define MESSAGE_LEN 512

// HASH_LEN = 41
#define HASH_LEN ((SHA_SUM_LEN) + 1)
#define MODE_LEN 7
#define PATH_LEN 256
#define FILE_LEN 128


#define ID_WIDTH 3
#define FILE_HASH_LEN ((HASH_LEN) + (ID_WIDTH) + 1)
#define MAX_SAME_ID 999


#endif