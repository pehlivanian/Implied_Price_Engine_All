//
// Created by charles on 10/13/16.
//

#include "QuoteSimulator.hpp"

int main(int argc, char **argv)
{
    QuoteSimulator<8>* QS = new QuoteSimulator<8>(100);
    QS->process();

    return 0;
}