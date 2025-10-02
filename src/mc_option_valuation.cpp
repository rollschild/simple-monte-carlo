#include "mc_option_valuation.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include <utility>
#include <vector>

#include "equity_price_generator.h"

MCOptionValuation::MCOptionValuation(OptionInfo&& opt, int time_steps,
                                     double vol, double int_rate,
                                     double div_rate, BarrierType barrier_type,
                                     double barrier_value)
    : opt_{std::move(opt)},
      time_steps_{static_cast<unsigned>(time_steps)},
      vol_{vol},
      int_rate_{int_rate},
      div_rate_{div_rate},
      barrier_type_{barrier_type},
      barrier_value_{barrier_value} {}

double MCOptionValuation::calc_price(double spot, int num_scenarios,
                                     unsigned unif_start_seed) {
    bool barrier_hit =
        (barrier_type_ == BarrierType::UP_AND_OUT && spot >= barrier_value_) ||
        (barrier_type_ == BarrierType::DOWN_AND_OUT && spot <= barrier_value_);

    if (barrier_hit) {
        return 0.0;  // option is worthless
    }

    if (opt_.time_to_expiration() > 0.0) {
        // Checks whether there is time left
        // Creates a 64-bit Mersenne Twister engine, that will drive a uniform
        // integer distribution over the interval of the minimum possible
        // unsigned value to the max value
        // Purpose is to generate random unsigned integer seed values to use as
        // the seed argument for each `EquityPriceGenerator` instance generating
        // a distinct scenario
        std::mt19937_64 mt_unif{unif_start_seed};
        std::uniform_int_distribution<unsigned> unif_int_dist{};

        std::vector<double> discounted_payoffs;
        discounted_payoffs.reserve(num_scenarios);
        const double disc_factor =
            std::exp(-int_rate_ * opt_.time_to_expiration());

        for (int i = 0; i < num_scenarios; ++i) {
            EquityPriceGenerator epg{spot,
                                     static_cast<int>(time_steps_),
                                     opt_.time_to_expiration(),
                                     vol_,
                                     int_rate_,
                                     div_rate_};

            // unif_int_dist(mt_unif) generates next seed
            auto scenario = epg(unif_int_dist(mt_unif));

            switch (barrier_type_) {
                case BarrierType::NONE:
                    break;
                case BarrierType::UP_AND_OUT: {
                    auto barrier_hit_pos =
                        std::find_if(scenario.cbegin(), scenario.cend(),
                                     [this](double sim_eq) {
                                         return sim_eq >= barrier_value_;
                                     });
                    if (barrier_hit_pos != scenario.cend()) {
                        barrier_hit = true;
                    }
                } break;
                case BarrierType::DOWN_AND_OUT: {
                    auto barrier_hit_pos =
                        std::find_if(scenario.cbegin(), scenario.cend(),
                                     [this](double sim_eq) {
                                         return sim_eq <= barrier_value_;
                                     });
                    if (barrier_hit_pos != scenario.cend()) {
                        barrier_hit = true;
                    }

                } break;
            }

            if (barrier_hit) {
                discounted_payoffs.push_back(0.0);
            } else {
                // For European options, only need the final price from each
                // scenario
                discounted_payoffs.push_back(
                    disc_factor * opt_.option_payoff(scenario.back()));
            }

            barrier_hit = false;
        }

        return (1.0 / num_scenarios) *
               std::accumulate(discounted_payoffs.cbegin(),
                               discounted_payoffs.cend(), 0.0);
    } else {
        // at expiration
        return opt_.option_payoff(spot);
    }
}
