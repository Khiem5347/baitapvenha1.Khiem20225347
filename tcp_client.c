#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Cach su dung: %s <Dia chi IP> <Cong>\n", argv[0]);
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Loi ket noi den server");
        return 1;
    }

    char buffer[1024];
    int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        printf("Server: %s\n", buffer);
    }

    printf("Nhap du lieu de gui (go 'exit' de thoat):\n");
    while (1) {
        printf("> ");
        fgets(buffer, sizeof(buffer), stdin);
        
        send(sock, buffer, strlen(buffer), 0);
        
        if (strncmp(buffer, "exit", 4) == 0) {
            break;
        }
    }

    close(sock);
    return 0;
}