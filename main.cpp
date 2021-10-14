#include <stdio.h>
#include <iostream>

#include "include/eventq.h"
#include "include/simobject.h"
#include "include/simulator.h"
#include "include/types.h"

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