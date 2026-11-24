#include "physics.h"
#include "util.h"
# include "switches.h"

bool isDirectlyConnected(int fromF, int fromR, int toF, int toR,
                         const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                         char& outConnType) {
    outConnType = CONN_NONE;
    if (fromF == toF){
        if (fromR == toR){
            return false;
        }
        if ((fromR + 1 == toR) || (fromR - 1 == toR)){
            if (fromR < toR){
                if (horizontalConn[fromF-1][fromR-1] == CONN_NONE || horizontalConn[fromF-1][fromR-1] == CONN_SOLID_WALL){
                    return false;
                } else {
                    outConnType = horizontalConn[fromF-1][fromR-1];
                    return true;
                }
            } else {
                if (horizontalConn[fromF-1][toR-1] == CONN_NONE || horizontalConn[fromF-1][toR-1] == CONN_SOLID_WALL){
                    return false;
                } else {
                    outConnType = horizontalConn[fromF-1][toR-1];
                    return true;
                }
            }
        }
    }
    if ((fromR == 1 && toR == 1) && ((fromF == 1 && toF == 2) || (fromF == 2 && toF == 1) || (fromF == 2 && toF == 3) ||
     (fromF == 3 && toF == 2) || (fromF == 4 && toF == 5) || (fromF == 5 && toF == 4))){
        outConnType = CONN_STAIRS;
        return true;
    }
    if (((fromR == 2 || fromR == 3) && (toR == 2 || toR == 3)) && ((fromF == 1 && toF == 2) || (fromF == 2 && toF == 1))) {
        outConnType = CONN_LIFT1;
        return true;
    }
    if ((fromR == 4 && toR == 4)  && ((fromF == 4 && toF == 5) || (fromF == 5 && toF == 4) || 
    (fromF == 2 && toF == 4) || (fromF == 4 && toF == 2) || (fromF == 5 && toF == 2) || (fromF == 2 && toF == 5))) {
        outConnType = CONN_LIFT2;
        return true;
    }
    return false;
}

bool canTraverse(int targetFloor, int targetRoom, char connType,
                 const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                 int emergencyTripsLeft, bool lift2Operational) {
    if (targetFloor < 1 || targetFloor > NUM_FLOORS || targetRoom < 1 || targetRoom > ROOMS_PER_FLOOR) {
        return false;
    }
    if (connType == CONN_STAIRS || connType == CONN_LIFT1 || connType == CONN_DOOR || connType == CONN_WINDOW) {
        return true;
    }
    if (connType == CONN_LOCKED_DOOR) {
        return unlockFloor3Door(inventory, inventoryCount);
    }
    if (connType == CONN_LIFT2) {
        return (emergencyTripsLeft > 0) || lift2Operational;
    }
    return false;
}

void applyResourceDelta(int& energy, int& time, int deltaEnergy, int deltaTime) {
    energy += deltaEnergy;
    time += deltaTime;
    energy = clampInt(energy, 0, MAX_ENERGY);
    time = clampInt(time, 0, MAX_TIME);
}

void applyScoreDelta(int& score, int deltaScore) {
    score += deltaScore;
}

bool hasItem(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount, char itemSymbol) {
    for (int i = 0; i < inventoryCount; i++) {
        if (inventory[i] == itemSymbol) {
            return true;
        }
    }
    return false;
}

bool addItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol) {
    if (inventoryCount >= MAX_INVENTORY_CAPACITY) {
        return false;
    } else {
        inventory[inventoryCount] = itemSymbol;
        inventoryCount++;
        return true;
    }
}

bool removeItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol) {
    for(int i=0; i<inventoryCount;i++){
        if(inventory[i]==itemSymbol){
            for(int j=i;j<inventoryCount-1;j++){
                inventory[j]=inventory[j+1];
            }
            inventory[inventoryCount-1]=ENT_NONE;
            inventoryCount--;
            return true;
        }
    }
    return false;
}
