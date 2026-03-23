#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void remove_newline(char *str) {
    str[strcspn(str, "\n")] = 0;
}

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
        perror("Loi ket noi");
        return 1;
    }

    char mssv[20], name[100], dob[20], gpa[10];
    
    printf("Nhap thong tin sinh vien:\n");
    printf("MSSV: "); fgets(mssv, sizeof(mssv), stdin); remove_newline(mssv);
    printf("Ho ten: "); fgets(name, sizeof(name), stdin); remove_newline(name);
    printf("Ngay sinh (YYYY-MM-DD): "); fgets(dob, sizeof(dob), stdin); remove_newline(dob);
    printf("Diem trung binh: "); fgets(gpa, sizeof(gpa), stdin); remove_newline(gpa);

    char buffer[512];
    snprintf(buffer, sizeof(buffer), "%s %s %s %s", mssv, name, dob, gpa);

    send(sock, buffer, strlen(buffer), 0);
    printf("Da gui thong tin thanh cong!\n");

    close(sock);
    return 0;
}