#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char * argv[]) {
    string fileName, hostname;
    int port;
    struct sockaddr_in addr;

    /* check and set arguments */
    if (argc < 4)
    {
        cout << "Wrong argument count" << endl;
        return -1;
    }

    fileName = argv[1];

    if (strcmp( argv[2], "localhost") == 0) { hostname = "127.0.0.1"; }
    else { hostname = argv[2]; }

    port = atoi(argv[3]);

    /* socket creation */
    int socketDescr = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescr < 0)
    {
        cout << "Socket creation failed" << endl;
        return -1;
    }

    /* socket addres */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(hostname.c_str());

    /* connection */
    if (connect(socketDescr, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        cout << "Connection failed" << endl;
        return -1;
    }

    /* openning file */
    FILE *fileDescr= fopen(fileName.c_str(),"rb");
    if (fileDescr == NULL)
    {
        cout << "Wrong file open" << endl;
        return -1;        
    }

    /* reading and sending file */
    /* sendfile(socketDescr, fileDescr, ); */

    char buffer[1024];
    int i = 0;
    while(!feof(fileDescr)) {
        int res = fread(buffer, sizeof(char), sizeof(buffer), fileDescr);
        int size = ftell(fileDescr);
        printf("bytes read: %d, part:%d, pos: %d \n", res, i, size);
        if(res != 0)
        {
            send(socketDescr, buffer, res, 0); 
        }

        i++;
    } /* */

    return 0;
}
