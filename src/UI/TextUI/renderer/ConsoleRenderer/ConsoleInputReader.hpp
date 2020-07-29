#pragma once
#include <sys/time.h>
#include <sys/types.h>
#include <UITextInput.hpp>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <termios.h>
#include <unistd.h>

class ConsoleInputReader
{
    struct termios oldSettings, newSettings;
    fd_set set;
    struct timeval tv;

    std::string tempBuffer;
    UITextInput& m_Input;

    constexpr static unsigned asciiEnterKeyApple = 10u;
    constexpr static unsigned asciiBackspaceKeyApple = 127u;
    constexpr static unsigned asciiEnterKey = 13u;
    constexpr static unsigned asciiBackspaceKey = 8u;

public:
    ConsoleInputReader(UITextInput& input) : m_Input(input) 
    { 
        setupInputReceiver();
        tempBuffer = std::string(m_Input.getBuffer().data());
    }

    ~ConsoleInputReader() { tcsetattr(fileno(stdin), TCSANOW, &oldSettings); }

    bool hasUnconsumedInput() const { return m_Input.getInputPendingFlag(); }

    void update()
    {
        /* wait till the input is consumed */
        if (m_Input.getInputPendingFlag()) return;
        if (pollUserInput())
        {
            onInputReceived();
        }
        printCurrentInput();
    }

private:
    bool pollUserInput()
    {
        setupUserPolling();
        const auto result = select(fileno(stdin) + 1, &set, NULL, NULL, &tv);
        if (result < 0)
        {
            /* runtime error */
        }

        auto wasInputProvided = [](const auto& res) { return res > 0; };
        return wasInputProvided(result);
    }

    void setupUserPolling()
    {
        tv.tv_sec = 0;
        tv.tv_usec = 10; /* timeout for user input */

        FD_ZERO(&set);
        FD_SET(fileno(stdin), &set);
    }

    void onInputReceived()
    {
        char c;
        read(fileno(stdin), &c, 1);
        updateBasedOnInput(c);
    }

    void updateBasedOnInput(const char& userInput) 
    {
        /* TODO: add backspace support */
        auto isKeyCode = [userInput](auto asciiKeyCode){
            return static_cast<const unsigned>(userInput) == asciiKeyCode;
         };

        if (isKeyCode(asciiBackspaceKey) or isKeyCode(asciiBackspaceKeyApple))
        {
            onBackspacePressed();
            return;
        } 
        
        if (isKeyCode(asciiEnterKey) or isKeyCode(asciiEnterKeyApple))
        {
            onEnterPressed();
            return; 
        }

        addToUserBuffer(userInput);
    }

    void onBackspacePressed()
    {
        if(tempBuffer.empty()) return;
        tempBuffer.pop_back();
        strcpy(m_Input.getBuffer().data(), tempBuffer.c_str());
    }

    void onEnterPressed()
    {
        m_Input.getInputPendingFlag() = true;
        tempBuffer.clear();
    }

    void addToUserBuffer(const char& userInput)
    {
        tempBuffer += userInput;
        strcpy(m_Input.getBuffer().data(), tempBuffer.c_str());
    }
    
    void printCurrentInput()
    {
        std::cout << "\33[2K\r" << tempBuffer << std::flush;
    }

    void setupInputReceiver()
    {
        tcgetattr(fileno(stdin), &oldSettings);
        newSettings = oldSettings;
        newSettings.c_lflag &= (~ICANON & ~ECHO);
        tcsetattr(fileno(stdin), TCSANOW, &newSettings);
    }
};
