//
// Created by charles on 10/10/16.
//

#include <gtest/gtest.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <thread>

#include "Client.hpp"
#include "QuoteServer.hpp"

TEST(ServerClientTest, Connect) {
    pid_t pid;
    int pipefd[2];

    // Form pipe for parent-child communication
    pipe(pipefd);

    // Form parent, child processes
    pid = fork();

    if (pid > 0) {
        // Parent
        int port = 50001, port_assigned;

        QuoteServer *QS = new QuoteServer( port);
        port_assigned = QS->get_port();

        write(pipefd[1], &port_assigned, sizeof(port_assigned));
        close(pipefd[1]);

        // Poll on separate thread
        auto t = std::thread(&QuoteServer::poll, QS, 1);
        t.join();

    }
    else {
        // Child
        int port;

        read(pipefd[0], &port, sizeof(port));
        close(pipefd[0]);

        sleep(2);

        Client *C = new Client(port, (char *)"0.0.0.0");
        auto r = C->fetch();

        ASSERT_EQ(C->get_bytes_read(), 49470);

    }
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
