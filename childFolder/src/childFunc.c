#include "childFunc.h"

int read_env_file(const char* filename, char** env_vars, int* env_count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Не удалось открыть файл: %s\n", filename);
        return -1;
    }
    
    char line[MAX_LINE_LEN];
    *env_count = 0;
    
    while (fgets(line, sizeof(line), file) && *env_count < MAX_ENV_VARS) {
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }

        if (line[0] == '#' || strlen(line) == 0) {
            continue;
        }
        
        env_vars[*env_count] = strdup(line);
        (*env_count)++;
    }
    
    fclose(file);
    return 0;
}

