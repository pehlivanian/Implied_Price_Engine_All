//
// Created by charles on 11/17/18.
//

#ifndef IMPLIED_PRICE_ENGINE_QUOTESERVER_H
#define IMPLIED_PRICE_ENGINE_QUOTESERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <cstring>
#include <utility>

class QuoteServer {
public:
    QuoteServer(int port) : port_(port),
                            listenfd_(0),
                            connfd_(0),
                            assigned_port_(-1)
    { filename_to_serve_ = new char(24); strcpy(filename_to_serve_, "serveme.dat"); bind_and_listen(); }
    QuoteServer(char* filename) : port_(0),
                                  listenfd_(0),
                                  connfd_(0),
                                  assigned_port_(-1)
    { filename_to_serve_ = new char(strlen(filename)); strcpy(filename_to_serve_, filename); }
    QuoteServer(char* filename, int port) : port_(port),
                                            listenfd_(0),
                                            connfd_(0),
                                            assigned_port_(-1)
    { filename_to_serve_ = new char(strlen(filename)); strcpy(filename_to_serve_, filename); bind_and_listen(); }
    int get_port() const { return assigned_port_; }

    void poll(short max_served=1);
private:
    int port_;
    int listenfd_;
    int connfd_;
    int assigned_port_;
    struct sockaddr_in serv_addr_;
    char* filename_to_serve_;

    void bind_and_listen();
    int send(int num, int fd);
};


#endif //IMPLIED_PRICE_ENGINE_QUOTESERVER_H
