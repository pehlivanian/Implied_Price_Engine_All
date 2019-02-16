#include "SimulateCurveView_2.hpp"

auto main(int argc, char **argv) -> int {

  // sim_batch_mode, sim_realtime_mode, sync_mode
  ImpliedServer<6> *IS = new ImpliedServer<6>(false, true, true);
  
  IS->process();
  
  IS->write_user_curve();
  IS->write_implied_curve();
  IS->write_merged_curve();

  return 0;
}
