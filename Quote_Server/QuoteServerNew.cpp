//
// Created by charles on 11/17/18.
//

#include "QuoteServerNew.hpp"

int main(int argc, char **argv) {

    if(argc != 2)
    {
        printf("\n Usage: %s <port of server> \n",argv[0]);
        return 1;
    }

    QuoteServer* QS = new QuoteServer(atoi(argv[1]));

    QS->poll();
}
