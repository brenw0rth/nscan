#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "scanner.h"

typedef struct
{
    pthread_mutex_t mutex;
    char* host;
    int* ports;
    int nports;
    int index;
}
PortData;

void tcp_connect_scan(char* host, int* ports, int nports, int nworkers)
{
    PortData portData;
    portData.host = host;
    portData.ports = ports;
    portData.nports = nports;
    portData.index = 0;
    pthread_mutex_init(&portData.mutex, NULL);
    
    pthread_t workers[nworkers];
    
    for (int i = 0; i < nworkers; ++i)
    {
        pthread_create(&workers[i], NULL, tcp_connect_worker, &portData);
    }
    
    for (int i = 0; i < nworkers; ++i)
    {
        pthread_join(workers[i], NULL);
    }
    
    pthread_mutex_destroy(&portData.mutex);
}

void* tcp_connect_worker(void* arg)
{
    PortData* portData = (PortData*)arg;
    char* host = portData->host;
    int port;
    
    while (1)
    {
        pthread_mutex_lock(&portData->mutex);
        if (portData->index >= portData->nports)
        {
            pthread_mutex_unlock(&portData->mutex);
            break;
        }
        
        port = portData->ports[portData->index++];
        pthread_mutex_unlock(&portData->mutex);

        if (tcp_connect(host, port))
        {
            printf("%s:%i - [OPEN]\n", host, port);
        }
    }
    
    pthread_exit(NULL);
}

int tcp_connect(char* host, int port)
{
    int sock_fd;
    struct sockaddr_in server_addr;
    int conn_accepted = 1;

    server_addr.sin_addr.s_addr = inet_addr(host);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1)
    {
        puts("could not create socket");
        return 0;
    }

    if (connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        conn_accepted = 0;
    }

    close(sock_fd);
    return conn_accepted;
}
