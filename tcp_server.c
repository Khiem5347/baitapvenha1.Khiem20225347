#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Cach su dung: %s <Cong> <File loi chao> <File luu noi dung>\n", argv[0]);
        return 1;
    }

    char greeting[1024] = {0};
    FILE *f_greet = fopen(argv[2], "r");
    if (f_greet) {
        fread(greeting, 1, sizeof(greeting) - 1, f_greet);
        fclose(f_greet);
    } else {
        strcpy(greeting, "Xin chao tu Server!\n"); 
    }

    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(atoi(argv[1]));

    bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_sock, 5);
    
    printf("TCP Server dang lang nghe o cong %s...\n", argv[1]);

    while (1) {
        socklen_t client_len = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
        
        printf("Client %s da ket noi.\n", inet_ntoa(client_addr.sin_addr));

       
        send(client_sock, greeting, strlen(greeting), 0);

        
        FILE *f_out = fopen(argv[3], "a");
        if (!f_out) {
            perror("Khong the mo file luu noi dung");
            close(client_sock);
            continue;
        }

        char buffer[1024];
        int bytes_received;
        while ((bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
            buffer[bytes_received] = '\0';
            
           
            if (strncmp(buffer, "exit", 4) == 0) break;
            
            printf("Client noi: %s", buffer);
            
      
            fputs(buffer, f_out);
            fflush(f_out); 
        }

        printf("Client da ngat ket noi.\n");
        fclose(f_out);
        close(client_sock);
    }

    close(server_sock);
    return 0;
}