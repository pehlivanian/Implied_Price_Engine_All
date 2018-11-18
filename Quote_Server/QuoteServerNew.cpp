//
// Created by charles on 11/17/18.
//

#include "QuoteServerNew.hpp"

int main(int argc, char **argv) {

    QuoteServer* QS = new QuoteServer(argv[1], atoi(argv[2]));

    QS->poll();
}
