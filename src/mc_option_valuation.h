#ifndef MC_OPTION_VALUATION_H
#define MC_OPTION_VALUATION_H

#include "option_info.h"

enum class BarrierType {
    NONE,
    UP_AND_OUT,
    DOWN_AND_OUT,
};

class MCOptionValuation {
   public:
    MCOptionValuation(OptionInfo&& opt, int time_steps, double vol,
                      double int_rate, double div_rate = 0.0,
                      BarrierType barrier_type = BarrierType::NONE,
                      double barrier_value = 0.0);
    double calc_price(double spot, int num_scenarios, unsigned unif_start_seed);

   private:
    OptionInfo opt_;
    unsigned time_steps_;
    double vol_, int_rate_, div_rate_;
    BarrierType barrier_type_;
    double barrier_value_;
};

#endif
