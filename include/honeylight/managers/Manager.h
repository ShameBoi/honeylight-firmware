/**
 * Manager.h
 *
 * @author Shame Boi
 */
#pragma once

class Manager {
public:
    Manager() = default;

    virtual ~Manager() = default;

    virtual void begin() = 0;

    virtual bool hasWork() = 0;

    virtual void work() = 0;
};
