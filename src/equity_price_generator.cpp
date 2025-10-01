#include "equity_price_generator.h"

#include <cmath>
#include <random>

EquityPriceGenerator::EquityPriceGenerator(double spot, int num_time_steps,
                                           double time_to_expiration,
                                           double volatility, double rf_rate,
                                           double div_rate)
    : spot_(spot),
      num_time_steps_(num_time_steps),
      time_to_expiration_(time_to_expiration),
      volatility_(volatility),
      rf_rate_(rf_rate),
      div_rate_(div_rate),
      dt_(time_to_expiration / num_time_steps) {}

std::vector<double> EquityPriceGenerator::operator()(unsigned seed) const {
    std::vector<double> v;
    v.reserve(num_time_steps_ + 1);

    std::mt19937_64 mt(seed);
    std::normal_distribution<> nd;

    auto new_price = [this](double prev_equity_price, double norm) {
        double price = 0.0;
        double exp_arg_1 =
            (rf_rate_ - div_rate_ - ((volatility_ * volatility_) / 2.0)) * dt_;
        double exp_arg_2 = volatility_ * norm * std::sqrt(dt_);

        price = prev_equity_price * std::exp(exp_arg_1 + exp_arg_2);

        return price;
    };

    v.push_back(spot_);
    double equity_price = spot_;

    for (int i = 1; i <= num_time_steps_; ++i) {
        equity_price = new_price(equity_price, nd(mt));  // norm = nd(mt)
        v.push_back(equity_price);
    }

    return v;
}
