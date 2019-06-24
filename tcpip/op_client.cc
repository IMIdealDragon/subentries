#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

const int BUF_SIZE = 1024;
const int RLT_SIZE = 4;
const int OPSZ = 4;//一个数据占四个字节

void error_handling(char* message);

int main(int argc, char *argv[])
{
        int sock;
        char opmsg[BUF_SIZE];//存储操作的数
        int result, opnd_cnt, i;
        struct sockaddr_in serv_adr;

        if(argc != 3) {
                printf("Usage : %s <IP> <port> \n", argv[0]);
                exit(1);
        }

        sock = socket(PF_INET, SOCK_STREAM, 0);
        if(sock == -1)
               printf("socket() error\n");

        memset(&serv_adr, 0, sizeof(serv_adr));
        serv_adr.sin_family = AF_INET;
        serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
        serv_adr.sin_port = htons(atoi(argv[2]));

        if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1){
               printf("connect error");
                        }
        else
            puts("conneted -------------------");
        
        fputs("Operand count: ", stdout);
        scanf("%d", &opnd_cnt);
        opmsg[0] = (char)opnd_cnt;//待计算的个数
        
        for(i = 0; i < opnd_cnt; i++)  //读入数据
        {
            printf("Operand %d: ", i+1);
            scanf("%d", (int*)&opmsg[i*OPSZ + 1]);
        }

        fgetc(stdin);
        fputs("Operator: ", stdout);
        scanf("%c", &opmsg[opnd_cnt*OPSZ+1]);
        write(sock, opmsg, opnd_cnt*OPSZ+2);
        read(sock, &result, RLT_SIZE);

        printf("Operation result: %d \n", result);
        close(sock);

        return 0;
}


void error_handling(char *message)
{
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}
