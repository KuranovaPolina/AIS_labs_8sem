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

#include "server.h"
#include <iomanip>

using namespace std;

void ThreadProc(Server *server, int tread_id) {
    int socketDescr = 0;
    while(1)
    {
        if ((*server->getCompAddr()).pop(&socketDescr))
        {
            string res = "Consumer (" + to_string(tread_id) + "): " + to_string(socketDescr) + "\n";
            cout << res; 

            time_t now;
            tm* datetime;
	        char datetime_str[100];

            time(&now);
	        datetime = localtime(&now);

            strftime(datetime_str, 100, "%d-%m-%Y:%H-%M-%S", datetime);
            string filename = server->getPath() + "file_" + datetime_str + ".txt";
            FILE *fileDescr= fopen(filename.c_str(), "w");
            
            if (fileDescr == NULL)
            {
                cout << "Wrong file open " << filename << endl;
                exit(EXIT_FAILURE);       
            }

            char buffer[1024];
            int bytes_read;
            do
            {
                bytes_read = recv(socketDescr, buffer, sizeof(buffer), 0);  
                if (bytes_read > 0) {
                    fprintf(fileDescr, "%s", buffer);
                    printf("Buffer: %d\n", bytes_read); 
                }
            }
            while (bytes_read > 0);

            cout << "New file location: " << filename << endl;

            fclose(fileDescr);
        }
    }
}

Server::Server(in_addr_t hostname, in_port_t port, int thread_count, string path)
{
    /* socket creation */
    socketDescr = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescr < 0)
    {
        cout << "Socket creation failed" << endl;
        exit(EXIT_FAILURE);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = hostname;
    addr.sin_port = port;

    CompQueue = new CompetitiveQueue();

    thraedPool = new thread[thread_count];
    for (int i = 0; i < thread_count; i++)
    {
        thraedPool[i] = std::thread(ThreadProc, this, i);
    }

    thraedCount = thread_count;

    this->path = path;
}

Server::~Server()
{
    close(socketDescr);

    for (int i = 0; i < thraedCount; i++)
    {
        thraedPool[i].join();
    }

    cout << socketDescr << endl; /*  */
}

void Server::start()
{
    /* bind socket and "name" (addres) */
    if (::bind(socketDescr, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        cout << "Bind failed" << endl;
        exit(EXIT_FAILURE);
    }

    /* Listen port */
    cout << "Listener on port " << ntohs(addr.sin_port) << endl;

    if (listen(socketDescr, 5) == -1)
    {
        cout << "Listening failed" << endl;
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        int newSocketDescr;
        struct sockaddr_in newAddr;
        socklen_t addrlen = (socklen_t)sizeof(newAddr);

        /* Creating new socket with client's params */
        if ((newSocketDescr = accept(socketDescr, (struct sockaddr *)&newAddr, &addrlen)) == -1)
        {
            cout << "Acception failed" << endl;
            exit(EXIT_FAILURE);
        }  

        cout << "New connection created: hostname: " << inet_ntoa(newAddr.sin_addr) << " port: " << ntohs(newAddr.sin_port) << endl;

        CompQueue->push(newSocketDescr);
    }
}


