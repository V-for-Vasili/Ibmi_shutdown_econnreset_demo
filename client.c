#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <errno.h>

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b));

#define TRANSFER_BYTES (1 << 10)

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in cli_addr, serv_addr;
    bzero((char *)&cli_addr, sizeof(cli_addr));
    cli_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0", (void *)&cli_addr.sin_addr.s_addr);
    cli_addr.sin_port = htons(0);

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", (void *)&serv_addr.sin_addr.s_addr);
    serv_addr.sin_port = htons(9123);

    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) < 0)
    {
        perror("can't open stream socket");
        exit(1);
    }
    int yes = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) < 0)
    {
        perror("can't setsockopt");
        exit(1);
    }
    if (bind(sock, (const struct sockaddr *)&cli_addr, sizeof cli_addr) < 0)
    {
        perror("can't bind local address");
        exit(1);
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("can't connect to server");
        exit(1);
    }

    pid_t cpid;
    static char buffer[103];
    char buf[74];
    int byte_sent = 0;
    int byte_recv = 0;
    int len, r;
    cpid = fork();
    if (cpid == 0)
    {
        while (1)
        {
            do
                len = recv(sock, buf, sizeof(buf), 0);
            while (len == -1 && errno == EINTR);
            if (len < 0){
                perror("recv() failed");
                exit(-1);
            }
            else if(len > 0)
            {
                byte_recv += len;
                printf("recv(%d) total recv (%d)\n", len, byte_recv);
            }
            else
            {
                exit(0);
            }
            
        }
    }
    else
    {
        while (1)
        {
            if (byte_sent < TRANSFER_BYTES)
            {
                int send_bytes = MIN(TRANSFER_BYTES - byte_sent, sizeof buffer);
                do
                    r = send(sock, buffer, send_bytes, 0);
                while (r == -1 && errno == EINTR);
                if (r < 0)
                {
                    if (!(errno == EAGAIN || errno == EINPROGRESS || errno == EWOULDBLOCK))
                        perror("send() failed");
                }
                else
                {
                    byte_sent += r;
                    printf("send(%d) total sent (%d)\n", r, byte_sent);
                }
            }
            else
            {
                shutdown(sock, SHUT_WR);
                exit(0);
            }
        }
    }
    close(sock);
    return 0;
}
