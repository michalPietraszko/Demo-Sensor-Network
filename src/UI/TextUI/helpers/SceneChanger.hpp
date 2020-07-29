#pragma once
#include <UIScene.hpp>
#include <memory>
#include <functional>
#include <type_traits>
#include <cassert>

class SceneChanger
{
    using scene_ptr_t = std::unique_ptr<UIScene>;
    using change_scene_fn_t = std::function<scene_ptr_t()>;

public:
    template<typename T>
    static void setNext()
    {
        instance().setNextImpl<T>();
    }

    static std::unique_ptr<UIScene> nextScene()
    {
        return instance().nextSceneImpl();
    }

protected:
    static SceneChanger& instance()
    {
        static SceneChanger manager;
        return manager;
    };

    template<typename T>
    void setNextImpl()
    {
        static_assert(std::is_base_of_v<UIScene, T>);
        changeSceneFn = std::make_unique<change_scene_fn_t>([](){
            return std::make_unique<T>();
        });
    }

    std::unique_ptr<UIScene> nextSceneImpl()
    {
        assert(changeSceneFn);
        return changeSceneFn->operator()();
    }

private:
    std::unique_ptr<change_scene_fn_t> changeSceneFn{nullptr};
};