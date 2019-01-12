#include "SimulateCurveView.hpp"

auto main(int argc, char **argv) -> int {

  pid_t pid;
  int pipefd[2];
  
  // Pipe for ipc
  pipe(pipefd);

  // File for quotes
  std::string ofile = "quote_stream.dat";

  pid =fork();
  
  if (pid > 0) {
    // Parent
    int port = 50001, port_assigned;

    // Generate quote stream
    std::cerr << "Generating quote stream...\n";
    std::ofstream ofs(ofile);
    QuoteSimulator<6> *QSim = new QuoteSimulator<6>(1000);
    QSim->process();
    QSim->dump_to_file(ofs);
    

    std::cerr << "Creating quote server...\n";
    QuoteServer *QS = new QuoteServer(ofile.c_str(), port);
    port_assigned = QS->get_port();

    write(pipefd[1], &port_assigned, sizeof(port_assigned));
    close(pipefd[1]);

    QS->poll(1);
  }
  else {
    // Child
    int port;

    read(pipefd[0], &port, sizeof(port));
    close(pipefd[0]);

    sleep(2);

    std::cerr << "Creating implied client...\n";
    ImpliedServer<6> *IS = new ImpliedServer<6>(true, false, port);
    IS->process();

    IS->write_merged_curve();
    
    return 0;
    
  }
  
  return 0;
}
