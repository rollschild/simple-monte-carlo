#include "option_info.h"

#include <memory>
#include <utility>

CallPayoff::CallPayoff(double strike) : strike_(strike) {}
PutPayoff::PutPayoff(double strike) : strike_(strike) {}

std::unique_ptr<Payoff> CallPayoff::clone() const {
    return std::make_unique<CallPayoff>(*this);
}

std::unique_ptr<Payoff> PutPayoff::clone() const {
    return std::make_unique<PutPayoff>(*this);
}

double CallPayoff::payoff(double price) const { return price; }

double PutPayoff::payoff(double price) const { return price; }

void OptionInfo::swap(OptionInfo& rhs) noexcept {
    std::swap(payoff_ptr_, rhs.payoff_ptr_);
    std::swap(time_to_exp_, rhs.time_to_exp_);
}

double OptionInfo::time_to_expiration() const { return time_to_exp_; }

OptionInfo::OptionInfo(const OptionInfo& rhs)
    : payoff_ptr_(rhs.payoff_ptr_->clone()),
      time_to_exp_(rhs.time_to_expiration()) {}

OptionInfo& OptionInfo::operator=(const OptionInfo& rhs) {
    OptionInfo(rhs).swap(*this);
    return *this;
}

// TODO
double OptionInfo::option_payoff(double spot) const { return spot; }
