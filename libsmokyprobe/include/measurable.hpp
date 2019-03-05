#ifndef SG_MEASURABLE_HPP
#define SG_MEASURABLE_HPP

#include <functional>
#include <future>

namespace smokygrill {

typedef enum class MeasurableType_t {
    VOLTAGE,
    CURRENT,
    POWER,
    ENERGY,
    TEMPERATURE
} MeasurableType_t;

class Measurable {


public:

    virtual ~Measurable();

    virtual inline MeasurableType_t getType() const noexcept = 0;

    virtual void get(std::function<void(double)> callback, unsigned int frequency) = 0;

    virtual std::future<double> get() = 0;

    inline bool isRunning() const noexcept {
        return this->callback_running;
    }

    void stop() const noexcept;

private:

    bool callback_running;

};

}   // namespace smokygrill

#endif // MEASURABLE_HPP
