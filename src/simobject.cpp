#include "simobject.h"

int SimpleObject::count = 0;

SimObject::SimObject(Simulator *s)
    : EventManager(s->eventq()), sim(s) {
    sim->regObj(this);
}

void SimpleObject::init() {
    EventFunctionWrapper *func = new EventFunctionWrapper(
        [this]{printf("@%d: Hello World!\n", n);}
    );
    schedule(func, n+5);
}