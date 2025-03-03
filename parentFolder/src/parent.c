#include "parentFunc.h"

int main(int argc, char* argv[], char* envp[]) {
    if (argc != 2) {
        fprintf(stderr, "Использование: %s <путь_к_файлу_env>\n", argv[0]);
        return 1;
    }
    
    const char* env_file_path = argv[1];
    
    char** parent_env = NULL;
    int env_count = 0;
    
    while (envp[env_count] != NULL) {
        env_count++;
    }
    
    parent_env = (char**)malloc(env_count * sizeof(char*));
    if (!parent_env) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return 1;
    }
    
    for (int i = 0; i < env_count; i++) {
        parent_env[i] = envp[i];
    }
    
    setlocale(LC_COLLATE, "C");
    qsort(parent_env, env_count, sizeof(char*), compare_strings);
    
    printf("Переменные окружения родительского процесса:\n");
    for (int i = 0; i < env_count; i++) {
        printf("%s\n", parent_env[i]);
    }
    printf("-----------------------------------\n");
    
    char* env_vars[MAX_ENV_VARS];
    int env_vars_count = 0;
    
    if (read_env_file(env_file_path, env_vars, &env_vars_count) != 0) {
        free(parent_env);
        return 1;
    }
    
    char* child_path = getenv("CHILD_PATH");
    if (child_path == NULL) {
        fprintf(stderr, "Переменная окружения CHILD_PATH не установлена\n");
        free(parent_env);
        for (int i = 0; i < env_vars_count; i++) {
            free(env_vars[i]);
        }
        return 1;
    }
    
    int child_counter = 0;
    char key_input;
    
    printf("Нажмите '+' для запуска дочернего процесса, '*' для запуска с другим окружением, 'q' для выхода\n");
    
    while (1) {
        key_input = getchar();
        
        if (getchar() == '\n') {
            continue;
        }
        
        if (key_input == 'q') {
            printf("Завершение работы программы\n");
            break;
        }
        else if (key_input == '+' || key_input == '*') {
            pid_t pid = fork();
            
            if (pid == -1) {
                fprintf(stderr, "Ошибка при создании дочернего процесса\n");
                continue;
            }
            
            if (pid == 0) {
                char child_name[16];
                snprintf(child_name, sizeof(child_name), "child_%02d", child_counter);
                
                char child_exe_path[256];
                snprintf(child_exe_path, sizeof(child_exe_path), "%s/child", child_path);
                
                char* child_args[3];
                child_args[0] = child_name;
                
                if (key_input == '+') {
                    child_args[1] = (char*)env_file_path;
                    child_args[2] = NULL;
                } else {
                    child_args[1] = NULL;
                }
                
                char** child_env = NULL;
                
                child_env = (char**)malloc((env_vars_count + 1) * sizeof(char*));
                if (!child_env) {
                    fprintf(stderr, "Ошибка выделения памяти для окружения\n");
                    exit(1);
                }
                
                for (int i = 0; i < env_vars_count; i++) {
                    char* var_name = env_vars[i];
                    char* var_value = getenv(var_name);
                    
                    char env_var[MAX_LINE_LEN];
                    if (var_value != NULL) {
                        snprintf(env_var, sizeof(env_var), "%s=%s", var_name, var_value);
                    } else {
                        snprintf(env_var, sizeof(env_var), "%s=", var_name);
                    }
                    
                    child_env[i] = strdup(env_var);
                    if (!child_env[i]) {
                        fprintf(stderr, "Ошибка выделения памяти\n");
                        exit(1);
                    }
                }
                child_env[env_vars_count] = NULL;
                
                execve(child_exe_path, child_args, child_env);
                
                fprintf(stderr, "Ошибка при выполнении execve: %s\n", strerror(errno));
                
                for (int i = 0; i < env_vars_count; i++) {
                    free(child_env[i]);
                }
                free(child_env);
                
                exit(1);
            } else {
                printf("Запущен дочерний процесс с PID: %d\n", pid);
                child_counter = (child_counter + 1) % 100;
                
                int status;
                waitpid(pid, &status, 0);
                printf("Дочерний процесс завершился с кодом: %d\n", WEXITSTATUS(status));
            }
        }
    }
    
    free(parent_env);
    for (int i = 0; i < env_vars_count; i++) {
        free(env_vars[i]);
    }
    
    return 0;
}
