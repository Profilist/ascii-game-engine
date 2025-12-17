module;
#include <time.h>

export module core.clock;

export namespace age {

class Clock {
public:
    explicit Clock(float tickDuration = 0.05f);

    float tick();
    void sleepUntilNextTick();
    float tickDuration() const noexcept;
    void reset();

private:
    float tickDuration_;
    long long lastTickNs_;

    static long long nowNs();
    static void sleepNs(long long ns);
};

}
