#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <queue>
#include <ctime>

#include "../CompetitiveQueue/CompetitiveQueue.h"

using namespace std;

class Server
{
    private:
        int socketDescr;
        struct sockaddr_in addr;
        thread* thraedPool;
        int thraedCount;
        CompetitiveQueue* CompQueue;
        string path;
        int maxFileSize;
    public:
        Server(in_addr_t hostname, in_port_t port, int thread_count, string path, int maxFileSize);
        ~Server();

        void start();
        CompetitiveQueue* getCompAddr()
        {
            return CompQueue;
        }

        string getPath()
        {
            return path;
        }

        int getMaxFileSize()
        {
            return maxFileSize;
        }
};