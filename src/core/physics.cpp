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
    (void)energy; (void)time; (void)deltaEnergy; (void)deltaTime; // TODO
}

void applyScoreDelta(int& score, int deltaScore) {
    (void)score; (void)deltaScore; // TODO
}

bool hasItem(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount, char itemSymbol) {
    (void)inventory; (void)inventoryCount; (void)itemSymbol; // TODO
    return false;
}

bool addItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol) {
    (void)inventory; (void)inventoryCount; (void)itemSymbol; // TODO
    return false;
}

bool removeItem(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount, char itemSymbol) {
    (void)inventory; (void)inventoryCount; (void)itemSymbol; // TODO
    return false;
}
