#include <stdio.h>
#include <iostream>

#include "src/event.h"
#include "src/simobject.h"
#include "src/simulator.h"
#include "src/types.h"

using namespace std;

int main() {
    Simulator *sim = new Simulator();
    SimpleObject *obj1 = new SimpleObject(sim);
    SimpleObject *obj2 = new SimpleObject(sim);
    SimpleObject *obj3 = new SimpleObject(sim);
    
    sim->initObj();
    sim->simulate();

    return 0;
}