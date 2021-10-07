#ifndef __SIMOBJECT_H__
#define __SIMOBJECT_H__

#include "event.h"
#include "simulator.h"

class Simulator;

class SimObject : public EventManager
{
private:
    Simulator *sim;

public:
    SimObject(Simulator *s);

    virtual void init() = 0;
};

class SimpleObject : public SimObject
{
private:
    static int count;
    int n;

public:
    SimpleObject(Simulator *s) : SimObject(s) { n = count; }

    void init();
};

#endif // __SIMOBJECT_H__