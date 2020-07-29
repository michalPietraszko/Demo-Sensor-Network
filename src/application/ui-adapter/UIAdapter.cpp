#include <UIAdapter.hpp>
#include <SensorNetworkApplication.hpp>

bool UIAdapter::isMainNodeCreated() const
{
    return m_App.isMainNodeCreated();
}

unsigned UIAdapter::getSensorInfo() const
{
    return m_App.getSensorInfo();
}

void UIAdapter::setupMainNode(unsigned cyclicBufferSize)
{
    m_App.setupMainNode(cyclicBufferSize);
}

void UIAdapter::setupSensor(std::string name, unsigned interval, unsigned startingValue)
{
    m_App.setupSensor(name, interval, startingValue);
}

void UIAdapter::startSystem()
{
    m_App.startSystem();
}

void UIAdapter::stopSystem()
{
    m_App.stopSystem();
}

bool UIAdapter::isAllSystemSetup() const
{
    return m_App.isAllSystemSetup();
}

bool UIAdapter::isSystemRunning() const
{
    return m_App.isSystemRunning();
}
