#include "simulator.h"

bool Simulator::findObj(SimObject *obj) {
    for (auto it = objList.begin(); it < objList.end(); ++it) {
        if (*it == obj) {
            return true;
        }
    }
    return false;
}

void Simulator::regObj(SimObject *obj) {
    if (!findObj(obj)) {
        objList.push_back(obj);
    }
}

void Simulator::initObj() {
    for (auto it = objList.begin(); it < objList.end(); ++it) {
        (*it)->init();
    }
}

void Simulator::simulate() {
    while (true) {
        if (!_eventq->empty()) {
            _eventq->proceed();
        }
        else {
            printf("Job Done!\n");
            break;
        }
    }
}