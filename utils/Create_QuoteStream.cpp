//
// Created by charles on 10/13/16.
//

#include "QuoteSimulator.hpp"

int main(int argc, char **argv)
{
    QuoteSimulator<2>* QS = new QuoteSimulator<2>(1000);
    QS->process();

    return 0;
}
