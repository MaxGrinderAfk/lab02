#include "childFunc.h"

int main(int argc, char* argv[], char* envp[]) {
    pid_t pid = getpid();
    pid_t ppid = getppid();
    
    printf("Имя программы: %s\n", argv[0]);
    printf("PID: %d\n", pid);
    printf("PPID: %d\n", ppid);
    printf("-----------------------------------\n");
    
    if (argc >= 2 && argv[1] != NULL) {
        const char* env_file_path = argv[1];
        char* env_vars[MAX_ENV_VARS];
        int env_vars_count = 0;
        
        if (read_env_file(env_file_path, env_vars, &env_vars_count) != 0) {
            return 1;
        }
        
        printf("Переменные окружения дочернего процесса (режим '+'):\n");
        for (int i = 0; i < env_vars_count; i++) {
            char* var_name = env_vars[i];
            char* var_value = getenv(var_name);
            
            if (var_value != NULL) {
                printf("%s=%s\n", var_name, var_value);
            } else {
                printf("%s= (не установлена)\n", var_name);
            }
            
            free(env_vars[i]);
        }
    } else {
        printf("Переменные окружения дочернего процесса (режим '*'):\n");
        int i = 0;
        while (envp[i] != NULL) {
            printf("%s\n", envp[i]);
            i++;
        }
    }
    
    return 0;
}
