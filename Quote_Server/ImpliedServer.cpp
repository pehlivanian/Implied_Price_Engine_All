//
// Created by charles on 12/6/18.
//

#include <iostream>

#include "ImpliedServer.hpp"

auto main(int argc, char **argv) -> int {

    int port = atoi(argv[1]);

    ImpliedServer<6> *IS = new ImpliedServer<6>(true, port);

    IS->process();

    IS->write_user_curve();
    IS->write_implied_curve();
    IS->write_merged_curve();

    return 0;
}