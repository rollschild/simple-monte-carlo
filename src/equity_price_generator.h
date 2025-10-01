#ifndef EQUITY_PRICE_GENERATOR_H
#define EQUITY_PRICE_GENERATOR_H

#include <vector>

class EquityPriceGenerator {
   public:
    EquityPriceGenerator(double spot, int num_time_steps,
                         double time_to_expiration, double volatility,
                         double rf_rate, double div_rate);

    // Returns simulated random path of equity share prices
    std::vector<double> operator()(unsigned seed) const;

   private:
    double spot_;
    int num_time_steps_;
    double time_to_expiration_;
    double volatility_;
    double rf_rate_;   // continuous risk-free rate
    double div_rate_;  // continuous dividend rate
    double dt_;        // delta t
};

#endif
