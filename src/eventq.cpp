#include <assert.h>
#include <iostream>

#include "eventq.h"


/* class EventTimeSLot */

EventTimeSlot::~EventTimeSlot() {
    while (head) {
        Event *temp = head->nextEvent;
        delete head;
        head = temp;
    }
}

void EventTimeSlot::insert(Event *e) {
    assert(e != nullptr);
    assert(head != nullptr); // otherwise this time slot should already be deleted

    /* check if e should be placed at the top of the queue */
    if (e->priority() >= head->priority()) {
        e->nextEvent = head;
        head = e;
        return;
    }

    /* iterate the queue and insert e */
    Event *curr = head;
    Event *next = head->nextEvent; // should check whether this is a nullptr

    while (next && e->priority() < next->priority()) {
        curr = next;
        next = next->nextEvent;
    }

    curr->nextEvent = e;
    e->nextEvent = next;
}

void EventTimeSlot::remove(Event *e) {
    assert(e != nullptr);
    assert(head != nullptr); // otherwise this time slot should already be deleted
    
    /* check if the event going to be deleted is at the top of the queue */
    Event *next;
    while (head == e) {
        next = head->nextEvent;
        head = next;
    }

    if (head == nullptr)
        return;

    /* iterate the queue and delete corresponding events */
    Event *prev = head;
    Event *curr = prev->nextEvent; // should check whether this is a nullptr

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
    assert(head != nullptr); // otherwise this time slot should already be deleted
    
    /* check if the event going to be deleted is at the top of the queue */
    Event *next;
    while (head->name() == name) {
        next = head->nextEvent;
        delete head;
        head = next;
    }

    if (head == nullptr)
        return;

    /* iterate the queue and delete corresponding events */
    Event *prev = head;
    Event *curr = prev->nextEvent; // should check whether this is a nullptr

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

void EventTimeSlot::DEBUG_PRINT() {
    assert(head != nullptr);

    std::cout << "---- Event Time Slot @ " << _tick << " -----" << std::endl;

    for (Event *e = head; e; e = e->nextEvent) {
        assert(e->tick() == _tick);
        std::cout << "> Event " << e->name() << std::endl;
    }

    std::cout << "------------------------------" << std::endl;
}


/* class EventQueue */

EventQueue::~EventQueue() {
    while (_head) {
        EventTimeSlot *temp = _head->nextTimeSlot;
        delete _head;
        _head = temp;
    }
}

void EventQueue::insert(Event *e) {
    assert(e != nullptr);

    /* check if e should be placed at the top of the queue */
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

    /* iterate the queue and insert e */
    EventTimeSlot *curr = _head;
    EventTimeSlot *next = curr->nextTimeSlot; // should check whether this is a nullptr

    while (next && e->tick() > next->tick()) {
        curr = next;
        next = next->nextTimeSlot;
    }

    if (next == nullptr || e->tick() < next->tick()) {
        EventTimeSlot *newTimeSlot = new EventTimeSlot(e);
        curr->nextTimeSlot = newTimeSlot;
        newTimeSlot->nextTimeSlot = next;
    }
    else {
        next->insert(e);
    }

    /* update tick info */
    updateTick();
}

void EventQueue::remove(Event *e) {
    assert(e != nullptr);

    /* check if the event going to be deleted is at the top of the queue */
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


    /* iterate the queue and insert e */
    EventTimeSlot *prev = _head;
    EventTimeSlot *curr = _head->nextTimeSlot; // should check whether this is a nullptr

    while (curr) {
        curr->remove(e);
        if (curr->empty()) { // if a time slot is empty, it should be manually deconstructed
            prev->nextTimeSlot = curr->nextTimeSlot;
            delete curr;
            curr = prev->nextTimeSlot;
        }
        else {
            prev = curr;
            curr = curr->nextTimeSlot;
        }
    }

    /* update tick info */
    updateTick();
}

void EventQueue::remove(const std::string &name) {
    /* check if the event going to be deleted is at the top of the queue */
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


    /* iterate the queue and insert e */
    EventTimeSlot *prev = _head;
    EventTimeSlot *curr = _head->nextTimeSlot; // should check whether this is a nullptr

    while (curr) {
        curr->remove(name);
        if (curr->empty()) { // if a time slot is empty, it should be manually deconstructed
            prev->nextTimeSlot = curr->nextTimeSlot;
            delete curr;
            curr = prev->nextTimeSlot;
        }
        else {
            prev = curr;
            curr = curr->nextTimeSlot;
        }
    }

    /* update tick info */
    updateTick();
}

bool EventQueue::updateTick() {
    EventTimeSlot *next;

    while (_head) {
        next = _head->nextTimeSlot;

        if (_head->empty()) {
            delete _head;
            _head = next;
        }
        else {
            break;
        }
    }

    if (_head == nullptr)
        return false;

    _curTick = _head->tick();
    return true;
}

void EventQueue::proceed() {
    Event *eHead = head();
    eHead->execute();
    remove(eHead);
}

void EventQueue::DEBUG_PRINT() {
    std::cout << "========== Event Queue ==========" << std::endl;

    for (EventTimeSlot *ets = _head; ets; ets = ets->nextTimeSlot) {
        ets->DEBUG_PRINT();
    }

    std::cout << "=================================" << std::endl << std::endl;
}


/* class EventManager */

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