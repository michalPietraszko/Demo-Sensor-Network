#pragma once

class UI {
public:
    virtual ~UI() = default;
    virtual void processUserInput() {}
    virtual void update() {}
    virtual void display() {}
};