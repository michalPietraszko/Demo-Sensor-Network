#pragma once
#include <future>
#include <memory>

class OneShotTimer {
public:
    void alert() { prom->set_value(); }

    void wait() { fut->wait(); }

    void reset() {
        prom.reset(nullptr);
        fut.reset(nullptr);
        prom = std::make_unique<std::promise<void>>();
        fut = std::make_unique<std::future<void>>(prom->get_future());
    }

private:
    std::unique_ptr<std::future<void>> fut;
    std::unique_ptr<std::promise<void>> prom;
};
