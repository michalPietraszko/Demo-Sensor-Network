#pragma once
#include <iostream>
#include <string>

class SensorNetworkApplication;
class UIAdapter
{
public:
    UIAdapter(SensorNetworkApplication& app) : m_App{app} {}

    bool isMainNodeCreated() const;
    unsigned getSensorInfo() const;
    bool isAllSystemSetup() const;
    bool isSystemRunning() const;

    void setupMainNode(unsigned cyclicBufferSize);
    void setupSensor(std::string name, unsigned interval, unsigned startingValue);

    void startSystem();
    void stopSystem();

private:
    SensorNetworkApplication& m_App;
};