#include <assert.h>

#include "event.h"


EventTimeSlot::~EventTimeSlot() {
    while (head) {
        Event *temp = head->nextEvent;
        delete head;
        head = temp;
    }
}

void EventTimeSlot::insert(Event *e) {
    assert(head != nullptr);

    if (e->priority() >= head->priority()) {
        e->nextEvent = head->nextEvent;
        head = e;
        return;
    }

    Event *curr = head;
    Event *next = head->nextEvent;

    while (next && e->priority() < next->priority()) {
        curr = next;
        next = next->nextEvent;
    }

    curr->nextEvent = e;
    e->nextEvent = next;
}

void EventTimeSlot::remove(Event *e) {
    assert(head != nullptr);
    
    Event *next;
    while (head == e) {
        next = head->nextEvent;
        head = next;
    }

    if (head == nullptr)
        return;

    Event *prev = head;
    Event *curr = prev->nextEvent;

    while (curr) {
        if (curr == e) {
            prev->nextEvent = curr->nextEvent;
            curr = prev->nextEvent;
        }
        else {
            prev = curr;
            curr = curr->nextEvent;
        }
    }
}

void EventTimeSlot::remove(const std::string &name) {
    assert(head != nullptr);
    
    Event *next;
    while (head->name() == name) {
        next = head->nextEvent;
        delete head;
        head = next;
    }

    if (head == nullptr)
        return;

    Event *prev = head;
    Event *curr = prev->nextEvent;

    while (curr) {
        if (curr->name() == name) {
            prev->nextEvent = curr->nextEvent;
            delete curr;
            curr = prev->nextEvent;
        }
        else {
            prev = curr;
            curr = curr->nextEvent;
        }
    }
}


EventQueue::~EventQueue() {
    while (_head) {
        EventTimeSlot *temp = _head->nextTimeSlot;
        delete _head;
        _head = temp;
    }
}

void EventQueue::insert(Event *e) {
    if (_head == nullptr || e->tick() < _head->tick()) {
        EventTimeSlot *newTimeSlot = new EventTimeSlot(e);
        newTimeSlot->nextTimeSlot = _head;
        _head = newTimeSlot;
        return;
    }
    else if (e->tick() == _head->tick()) {
        _head->insert(e);
        return;
    }

    EventTimeSlot *curr = _head;
    EventTimeSlot *next = curr->nextTimeSlot;

    while (next && e->tick() > next->tick()) {
        curr = next;
        next = next->nextTimeSlot;
    }

    if (e->tick() < next->tick()) {
        EventTimeSlot *newTimeSlot = new EventTimeSlot(e);
        curr->nextTimeSlot = newTimeSlot;
        newTimeSlot->nextTimeSlot = next;
    }
    else {
        next->insert(e);
    }

    updateTick();
}

void EventQueue::remove(Event *e) {
    EventTimeSlot *next;

    while (_head) {
        next = _head->nextTimeSlot;

        _head->remove(e);
        if (_head->empty()) {
            delete _head;
            _head = next;
        }
        else {
            break;
        }
    }

    if (_head == nullptr)
        return;

    EventTimeSlot *prev = _head;
    EventTimeSlot *curr = _head->nextTimeSlot;

    while (curr) {
        curr->remove(e);
        if (curr->empty()) {
            prev->nextTimeSlot = curr->nextTimeSlot;
            delete curr;
            curr = prev->nextTimeSlot;
        }
        else {
            prev = curr;
            curr = curr->nextTimeSlot;
        }
    }

    updateTick();
}

void EventQueue::remove(const std::string &name) {
    EventTimeSlot *next;

    while (_head) {
        next = _head->nextTimeSlot;

        _head->remove(name);
        if (_head->empty()) {
            delete _head;
            _head = next;
        }
        else {
            break;
        }
    }

    if (_head == nullptr)
        return;

    EventTimeSlot *prev = _head;
    EventTimeSlot *curr = _head->nextTimeSlot;

    while (curr) {
        curr->remove(name);
        if (curr->empty()) {
            prev->nextTimeSlot = curr->nextTimeSlot;
            delete curr;
            curr = prev->nextTimeSlot;
        }
        else {
            prev = curr;
            curr = curr->nextTimeSlot;
        }
    }

    updateTick();
}

void EventQueue::updateTick() {
    if (_head == nullptr)
        return;
    
    EventTimeSlot *next = _head->nextTimeSlot;
    while (_head->empty()) {
        delete _head;
        _head = next;
        next = _head->nextTimeSlot;
    }

    if (_head == nullptr)
        return;

    _curTick = _head->tick();
}

void EventQueue::proceed() {
    Event *eHead = head();
    eHead->execute();
    remove(eHead);
}


Tick EventManager::curTick() {
    return _eventq->curTick();
}

void EventManager::schedule(Event *e, Tick t) {
    assert(t >= curTick());
    e->setTick(t);
    _eventq->insert(e);
}

void EventManager::schedule(Event *e, Tick t, const std::string &n) {
    assert(t >= curTick());
    e->setName(n);
    e->setTick(t);
    _eventq->insert(e);
}

void EventManager::deschedule(Event *e) {
    _eventq->remove(e);
}

void EventManager::deschedule(const std::string &n) {
    _eventq->remove(n);
}