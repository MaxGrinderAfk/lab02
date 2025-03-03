#include "libsChild.h"

#define MAX_ENV_VARS 100
#define MAX_LINE_LEN 256

int read_env_file(const char* filename, char** env_vars, int* env_count);

