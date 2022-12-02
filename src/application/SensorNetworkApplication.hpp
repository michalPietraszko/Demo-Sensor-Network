#pragma once
#include <AppLifetimeInfo.hpp>
#include <ComponentSpawner.hpp>
#include <Environment.hpp>
#include <LoopTimingController.hpp>
#include <SystemBroadcaster.hpp>
#include <SystemComponents.hpp>
#include <SystemId.hpp>
#include <SystemIdPool.hpp>
#include <TimerService.hpp>
#include <UI.hpp>
#include <UIAdapter.hpp>
#include <main-node/MainNode.hpp>
#include <sensor/Sensor.hpp>
#include <thread>

class SensorNetworkApplication {
public:
    unsigned getSensorInfo() const { return components.getNumOfSensors(); }
    bool isMainNodeCreated() const { return components.isMainNodeCreated(); }
    bool isAllSystemSetup() const { return components.isAllSystemSetup(); }
    bool isSystemRunning() const { return appInfo.isSensorNetworkSimulationRunning; }

public:
    void setupMainNode(unsigned cyclicBufferSize) {
        const auto& mainNode = components.addMainNode(cyclicBufferSize);
        ComponentSpawner::spawnRunnable<MainNode>(mainNode.id, cyclicBufferSize);
    }

    void setupSensor(const std::string name, const unsigned interval, const unsigned startingValue) {
        const auto& sensor = components.addSensor(name, interval, startingValue);
        ComponentSpawner::spawnRunnable<Sensor>(sensor.id, components.getMainNode().id, startingValue);

        auto& timer = addPeriodic(
            interval, [& msgr = this->systemBroadcaster, id = sensor.id]() { msgr.sendSensorReportTimerEvent(id); });

        startSensorIfSystemRunning(sensor.id, timer);
    }

    void setupObserver() {}

    void startSystem() {
        timerService.start();
        systemBroadcaster.broadcastStart();
        appInfo.isSensorNetworkSimulationRunning = true;
    }

    void stopSystem() {
        timerService.stop();
        appInfo.isAppRunning = false;
        if (not appInfo.isSensorNetworkSimulationRunning) {
            return;
        }

        systemBroadcaster.boradcastStop();
        appInfo.isSensorNetworkSimulationRunning = false;
    }

    void run() {
        while (appInfo.isAppRunning) {
            LoopTimingController ctr{};
            timerService.update();
            ui->processUserInput();
            ui->update();
            ui->display();
        }
        waitForEventsProcessing();
    }

    void addUI(std::unique_ptr<UI>&& newUi) { ui = std::move(newUi); }

    UIAdapter& getUIAdapter() {
        static UIAdapter uiAdapter(*this);
        return uiAdapter;
    }

private:
    Timer& addPeriodic(const unsigned period, TimerService::on_timeout_fn_t&& fn) {
        return timerService.add(Timer::timeout_ms_t{period}, Timer::TimerType::periodic, std::move(fn));
    }

    void startSensorIfSystemRunning(const SystemId& id, Timer& reportTimer) {
        if (not appInfo.isSensorNetworkSimulationRunning) return;
        systemBroadcaster.sendStart(id);
        reportTimer.start();
    }

    void waitForEventsProcessing() { std::this_thread::sleep_for(std::chrono::seconds(1)); }

    std::unique_ptr<UI> ui{nullptr};

    AppLifetimeInfo appInfo;
    TimerService timerService;
    SystemComponents components;
    SystemBroadcaster systemBroadcaster{components};
};