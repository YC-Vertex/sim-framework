#ifndef __EVENT_H__
#define __EVENT_H__

#include <string>
#include <functional>

#include "types.h"

class EventBase;
class Event;
class EventWrapper;

class EventTimeSlot;
class EventQueue;

class EventManager;


class EventBase
{
protected:
    static const EventFlag Schedule_Flag = 0x01;
    
public:
    static const Priority Minimum_Pri = -128;
    static const Priority Default_Pri = 0;
    static const Priority Maximum_Pri = 127;
};


class Event : public EventBase
{
    friend class EventTimeSlot;
    friend class EventQueue;

private:
    Event *nextEvent;

    std::string _name;

    Tick _tick;
    Priority _priority;
    EventFlag flag;

public:
    Event(Priority p = Default_Pri, EventFlag f = 0, const std::string &n = "")
        : _tick(0), _priority(p), flag(f), _name(n),
          nextEvent(nullptr)
    { }

    virtual void execute() = 0;

    std::string name()  { return _name; }
    Tick tick()         { return _tick; };
    Priority priority() { return _priority; }

    void setName(const std::string &n)  { _name = n; }
    void setTick(Tick t)                { _tick = t; }
    void setPriority(Priority p)        { _priority = p; }

    bool scheduled() { return flag & Schedule_Flag; }
};


class EventFunctionWrapper : public Event
{
private:
    std::function<void(void)> callback;
    std::string _name;

public:
    EventFunctionWrapper(const std::function<void(void)> &callback, Priority p = Default_Pri)
        : Event(p), callback(callback)
    { }

    void execute() { callback(); }
};


class EventTimeSlot
{
    friend class EventQueue;

private:
    Event *head;
    EventTimeSlot *nextTimeSlot;

    Tick _tick;

public:
    EventTimeSlot(Event *e)
        : head(e), _tick(e->_tick),
          nextTimeSlot(nullptr)
    { }
    ~EventTimeSlot();

    Tick tick() { return _tick; }
    
    void insert(Event *e);
    void remove(Event *e);
    void remove(const std::string &name);
    bool empty() { return head == nullptr; }
};


class EventQueue
{
private:
    EventTimeSlot *_head;

    Tick _curTick;

    void updateTick();

public:
    EventQueue() : _head(nullptr), _curTick(0) { };
    ~EventQueue();

    Event *head() { return _head->head; }

    void insert(Event *e);
    void remove(Event *e);
    void remove(const std::string &name);
    bool empty() { return _head == nullptr; }

    Tick curTick() { return _curTick; }

    void proceed();
};


class EventManager
{
private:
    EventQueue *_eventq;

public:
    EventManager(EventQueue *eq) : _eventq(eq) { }

    EventQueue *eventq() { return _eventq; }

    void schedule(Event *e, Tick t);
    void schedule(Event *e, Tick t, const std::string &n);
    void deschedule(Event *e);
    void deschedule(const std::string &n);

    Tick curTick();
};

#endif // __EVENT_H__
