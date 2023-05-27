#ifndef SCANNER_H
#define SCANNER_H

void tcp_connect_scan(char* host, int* ports, int nports, int nworkers);
void* tcp_connect_worker(void* arg);
int tcp_connect(char* host, int port);

#endif