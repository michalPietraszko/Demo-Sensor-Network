#include <SensorNetworkApplication.hpp>
#include <TextUI/TextUI.hpp>
#include <memory>

int main() {
    auto app    = std::make_unique<SensorNetworkApplication>();
    auto textUI = std::make_unique<TextUI>(app->getUIAdapter());
    app->addUI(std::move(textUI));
    app->run();

    // TODO Add static error checker to Logger
}
