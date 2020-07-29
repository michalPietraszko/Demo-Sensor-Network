#pragma once
#include <UserInfoView.hpp>
#include <UserInputView.hpp>

struct UserIO
{
    UserInfoView appStatus{};
    UserInputView userInput{};
};