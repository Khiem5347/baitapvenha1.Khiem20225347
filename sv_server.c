#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Cach su dung: %s <Cong> <File log>\n", argv[0]);
        return 1;
    }

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_sock, 5);
    
    printf("SV Server dang lang nghe o cong %s...\n", argv[1]);

    while (1) {
        socklen_t client_len = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
        
        
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), ip, INET_ADDRSTRLEN);

       
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm);

        char buffer[512];
        int bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            
            char log_entry[1024];
            snprintf(log_entry, sizeof(log_entry), "%s %s %s\n", ip, time_str, buffer);
            
            printf("Nhan moi: %s", log_entry);
            
            FILE *f_log = fopen(argv[2], "a");
            if (f_log) {
                fputs(log_entry, f_log);
                fclose(f_log);
            } else {
                perror("Loi mo file log");
            }
        }

        close(client_sock);
    }

    close(server_sock);
    return 0;
}