#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

#undef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b));

#define TRANSFER_BYTES (1 << 10)

int main(int argc, char *argv[])
{
    int sock, newsockfd;
    socklen_t clilen;
    struct sockaddr_in cli_addr, serv_addr;
    char buf[74];
    int len, r;

    /* open a TCP socket (an Internet stream socket) */
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) < 0)
    {
        perror("can't open stream socket");
        exit(1);
    }

    /* bind the local address, so that the cliend can send to server */
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", (void *)&serv_addr.sin_addr.s_addr);
    serv_addr.sin_port = htons(9123);

    int yes = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) <0)
    {
        perror("can't setsockopt");
        exit(1);
    }

    if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("can't bind local address");
        exit(1);
    }

    /* listen to the socket */
    listen(sock, 100);

    /* wait for a connection from a client; this is an iterative server */
    clilen = sizeof(cli_addr);
    newsockfd = accept(sock, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
        perror("can't bind local address");

    static char buffer[103];
    int byte_sent = 0;
    int byte_recv = 0;

    while (1)
    {
        if (byte_sent < TRANSFER_BYTES)
        {
            int send_bytes = MIN(TRANSFER_BYTES - byte_sent, sizeof buffer);
            do
                r = send(newsockfd, buffer, send_bytes, 0);
            while (r == -1 && errno == EINTR);
            if (r < 0)
            {
                if (!(errno == EAGAIN || errno == EINPROGRESS || errno == EWOULDBLOCK))
                {
                    perror("send() failed");
                    exit(-1);
                }
            }
            else
            {
                byte_sent += r;
                printf("send(%d) total sent (%d)\n", r, byte_sent);
            }
        }
        else
        {
            shutdown(newsockfd, SHUT_WR);
            break;
        }
    }

    sleep(1);

    while (1)
    {
        do
            len = recv(newsockfd, buf, sizeof(buf), 0);
        while (len == -1 && errno == EINTR);
        if (len < 0)
        {
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
            break;
        }
    }

    close(newsockfd);
    return 0;
}
