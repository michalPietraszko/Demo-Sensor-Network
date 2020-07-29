#pragma once

#include <functional>
#include <vector>

template<typename InputProcessFn>
class InputProcessor
{
public:
    void add(InputProcessFn fn)
    {
        inputsToProcess.push_back(fn);
    }

    template<typename... Args>
    void processAllInput(Args&&... args)
    {
        for(auto& inputProcessFn : inputsToProcess)
        {
            inputProcessFn(args...);
        }
    }

private:
    std::vector<InputProcessFn> inputsToProcess;
};