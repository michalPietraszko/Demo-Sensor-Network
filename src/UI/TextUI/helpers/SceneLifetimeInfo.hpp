#pragma once

class SceneLifetimeInfo
{
public:
    void setSceneChangeFlag()
    {
        m_ShouldChangeScene = true;
    }

    bool shouldChangeScene() const
    {
        return m_ShouldChangeScene;
    }

private:
    bool m_ShouldChangeScene{false};
};