//
// Created by charles on 10/13/16.
//

#include "SimulateQuotes.hpp"

int main(int argc, char **argv)
{

  std::ofstream ofs("quote_stream.dat");
  QuoteSimulator<24>* QS = new QuoteSimulator<24>(1000);
  QS->process();
  QS->dump_to_file(ofs);
  
  return 0;
}
