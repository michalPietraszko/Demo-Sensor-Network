#pragma once
#include <UITextField.hpp>
#include <BasicFormater.hpp>
#include <Renderer.hpp>

class UserInfoView
{
public:
    UITextField& get()
    {
        return userInfo;
    }

    void display(Renderer& renderer)
    {
        renderer.submit(userInfo, BasicFormater::any);
    }

private:
    UITextField userInfo;
};