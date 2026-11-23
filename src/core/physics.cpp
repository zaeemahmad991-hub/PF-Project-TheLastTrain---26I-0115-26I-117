#include "physics.h"
#include "util.h"

bool isDirectlyConnected(int fromF, int fromR, int toF, int toR,
                         const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                         char& outConnType) {
    (void)fromF; (void)fromR; (void)toF; (void)toR; (void)horizontalConn; // TODO
    outConnType = CONN_NONE;
    return false;
}

bool canTraverse(int targetFloor, int targetRoom, char connType,
                 const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                 int emergencyTripsLeft, bool lift2Operational) {
    (void)targetFloor; (void)targetRoom; (void)connType; (void)inventory;
    (void)inventoryCount; (void)emergencyTripsLeft; (void)lift2Operational; // TODO
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
            for(int j=i;j<inventoryCount;j++){
                inventory[j]=inventory[j+1];
            }
            inventory[i]=ENT_NONE;
            inventoryCount--;
            return true;
        }
    }
    return false;
}
