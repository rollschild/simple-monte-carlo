#ifndef OPTION_INFO_H
#define OPTION_INFO_H

#include <memory>
class Payoff {
   public:
    virtual double payoff(double price) const = 0;
    virtual std::unique_ptr<Payoff> clone() const = 0;
    virtual ~Payoff() = default;
};

class CallPayoff final : public Payoff {
   public:
    CallPayoff(double strike);
    double payoff(double price) const override;
    std::unique_ptr<Payoff> clone() const override;

   private:
    double strike_;
};

class PutPayoff final : public Payoff {
   public:
    PutPayoff(double strike);
    double payoff(double price) const override;
    std::unique_ptr<Payoff> clone() const override;

   private:
    double strike_;
};

class OptionInfo {
   public:
    OptionInfo(std::unique_ptr<Payoff> payoff, double time_to_exp);
    double option_payoff(double spot) const;
    double time_to_expiration() const;
    void swap(OptionInfo& rhs) noexcept;

    OptionInfo(const OptionInfo& rhs);             // user-defined
    OptionInfo& operator=(const OptionInfo& rhs);  // user-defined

    // Remaining rule of five

    // Default move constructor
    OptionInfo(OptionInfo&& rhs) = default;

    // Default move assignment
    OptionInfo& operator=(OptionInfo&& rhs) = default;

    // Default destructor
    ~OptionInfo() = default;

   private:
    std::unique_ptr<Payoff> payoff_ptr_;
    double time_to_exp_;
};

#endif
