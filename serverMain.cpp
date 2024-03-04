#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "serverModule/server.h"

using namespace std;

void sigtermHeandler(int sigNum);
void sighupHeandler(int sigNum);

/* hostname port  maxThreadsCount path maxfileSize*/

int main(int argc, char * argv[]) {
    string hostname, path;
    int port, maxThreadsCount;

    /* check and set arguments */
    if (argc < 6)
    {
        cout << "Wrong argument count" << endl;
        return -1;
    }

    if (strcmp( argv[1], "localhost") == 0) { hostname = "127.0.0.1"; }
    else { hostname = argv[1]; }

    port = atoi(argv[2]);

    maxThreadsCount = atoi(argv[3]);

    path = argv[4];

    /* socket creation */
    Server server = Server(inet_addr(hostname.c_str()), htons(port), maxThreadsCount, path);

    signal(SIGTERM, sigtermHeandler);
    signal(SIGHUP, sighupHeandler);

    cout << "PID: " << getpid() << endl;

    server.start();

}

void sigtermHeandler(int sigNum)
{
    cout << "Caught signal " << sigNum << ", coming out..." << endl;
    exit(0);
}

void sighupHeandler(int sigNum)
{
    cout << "Caught signal " << sigNum << ", coming out..." << endl;
    exit(0);
}
