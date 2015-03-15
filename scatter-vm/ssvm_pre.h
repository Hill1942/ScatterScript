#include "../scatter-asm/ssasm_pre.h"
#define TRUE  1
#define FALSE 0

#define LOAD_OK                        0
#define LOAD_ERROR_FAIL_FILE_OPEN      1
#define LOAD_ERROR_INVALID_SSE         2
#define LOAD_ERROR_UNSOPPORTED_VERSION 3
#define LOAD_ERROR_OUT_OF_MEMORY       4
#define LOAD_ERROR_OUT_OF_THREAD       5

#define DEFAULT_STACK_SIZE             1024

#define MAX_VALUE_STRING_SIZE          4096
#define MAX_THREAD_NUMBER              1024

