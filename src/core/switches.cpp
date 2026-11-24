#include "switches.h"
#include "util.h"
# include "physics.h"

bool canUseLift1(int fromFloor, int toFloor) {
    if ((fromFloor == 1 && toFloor == 2) || (fromFloor == 2 && toFloor == 1)){
        return true;
    }
    return false;
}

bool canUseLift2(int fromFloor, int toFloor, bool lift2Operational, int emergencyTripsLeft) {
    if (((fromFloor == 4 && toFloor == 5) || (fromFloor == 5 && toFloor == 4) || (fromFloor == 2 && toFloor == 4)
    || (fromFloor == 4 && toFloor == 2) || (fromFloor == 5 && toFloor == 2) || (fromFloor == 2 && toFloor == 5))){
        if (lift2Operational || emergencyTripsLeft > 0){
            return true;
        }
    }
    return false;
}

bool unlockFloor3Door(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount) {
    return hasItem(inventory, inventoryCount, ENT_MASTER_KEY);
}

bool installFuseControlPanel(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                             bool objectives[NUM_OBJECTIVES], int& score) {
    if (!hasItem(inventory, inventoryCount, ENT_FUSE)){
        return false;
    }
    if (objectives[OBJ_FUSE_INSTALLED]) {
        return false;
    }
    removeItem(inventory, inventoryCount, ENT_FUSE);
    objectives[OBJ_FUSE_INSTALLED] = true;
    applyScoreDelta(score, SCORE_INSTALL_FUSE);
    if (objectives[OBJ_LIFT_PASS]) {
        objectives[OBJ_LIFT2_ACTIVE] = true;
    }
    return true;
}

bool verifyCabinetCode(const char* enteredCode, const bool objectives[NUM_OBJECTIVES]) {
    if ((objectives[OBJ_CLUE_4] == false) || (objectives[OBJ_CLUE_2] == false) || (objectives[OBJ_CLUE_7] == false)) {
        return false;
    }
    if (stringEquals(enteredCode, "427")) {
        return true;
    }
    return false;
}
