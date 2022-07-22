//
// Created by charles on 11/14/20.
//

#include "SecPairExamples.hpp"

auto main(int argc, char **argv) -> int {

    auto sp1 = SecPair{8, 9, 1};
    auto sp2 = SecPair{8, 9, -1};

    sp2 *= -1;

    if (sp1 == sp2) {
        std::cout << "SecPair objects equal" << std::endl;
    } else {
        std::cout << "SecPair objects unequal" << std::endl;
    }

    return 0;

}

