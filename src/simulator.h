#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

#include <vector>

#include "event.h"
#include "simobject.h"

class SimObject;

class Simulator
{
private:
    EventQueue *_eventq;
    std::vector<SimObject*> objList;

public:
    Simulator() : _eventq(new EventQueue()) { }
    ~Simulator();

    bool findObj(SimObject *obj);
    void regObj(SimObject *obj);
    void initObj();
    void simulate();

    EventQueue *eventq() { return _eventq; }
};

#endif // __SIMULATOR_H__