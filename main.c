#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "scanner.h"

const char* USAGE = "Usage: nscan HOST [PORT, PORT...]\n";

int main(int argc, char* argv[])
{
    char* host = "";
    char** aports;
    int nports;
    int nworkers;
    int opt;

    while ((opt = getopt(argc, argv, "h:")) != -1)
    {
        switch (opt)
        {
            case 'h':
                puts(USAGE);
                return EXIT_SUCCESS;
            case '?':
                fprintf(stderr, "Invalid option: -%c\n", optopt);
                return EXIT_FAILURE;
        }
    }

    host = argv[optind];
    if (strcmp(host, "localhost") == 0)
    {
        host = "127.0.0.1";
    }

    nports = argc - (optind + 1);
    aports = &argv[optind + 1];
    nworkers = 4;

    int iports[nports];
    for (int i = 0; i < nports; i++)
    {
        iports[i] = atoi(aports[i]);
    }

    puts("[NSCAN v1.0]");
    puts("NETWORK SCANNER");
    puts("-------");
    tcp_connect_scan(host, iports, nports, nworkers);
    puts("-------");
    puts("SCAN FINISHED");

    return EXIT_SUCCESS;
}
