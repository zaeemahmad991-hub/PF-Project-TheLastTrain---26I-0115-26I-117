#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"
#include <iostream>
# include <cstring>
# include <cstdlib>
using namespace std;

void removeFromFloor(char shelf[], int &count, char sym){
    for (int i = 0; i < count; i++){
        if (shelf[i] == sym){
            for (int j = i; j < count - 1; j++){
                shelf[j] = shelf[j + 1];
            }
            count--;
            return;
        }
    }
}

void runSimulation(int& playerFloor, int& playerRoom,
                   int& energy, int& time, int& score,
                   char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                   bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                   char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                   char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                   int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]) {
    if (entityCounts[0][3]<MAX_ENTITIES_PER_ROOM){
        roomEntities[0][3][entityCounts[0][3]] = ENT_ENERGY_PACK;
        entityCounts[0][3]++;
    } 
    // TODO
    std::cout << "[stub] runSimulation not implemented yet\n";
}

bool handleMoveCommand(int targetFloor, int targetRoom,
                       int& currentFloor, int& currentRoom,
                       int& energy, int& time,
                       const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                       const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                       int& emergencyTripsLeft, bool lift2Operational) {
    (void)targetFloor; (void)targetRoom; (void)currentFloor; (void)currentRoom;
    (void)energy; (void)time; (void)horizontalConn; (void)inventory; (void)inventoryCount;
    (void)emergencyTripsLeft; (void)lift2Operational; // TODO
    return false;
}

bool handleInspectCommand(int currentFloor, int currentRoom,
                          int& energy, int& time,
                          const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int entityCount,
                          bool objectives[NUM_OBJECTIVES], int& score) {
    (void)currentFloor; (void)currentRoom; (void)energy; (void)time;
    (void)roomEntities; (void)entityCount; (void)objectives; (void)score; // TODO
    return false;
}

bool handleCollectCommand(char itemSymbol, int currentFloor, int currentRoom,
                          int& energy, int& time, int& score,
                          char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int& entityCount,
                          char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                          bool objectives[NUM_OBJECTIVES]) {
    (void)itemSymbol; (void)currentFloor; (void)currentRoom; (void)energy; (void)time;
    (void)score; (void)roomEntities; (void)entityCount; (void)inventory;
    (void)inventoryCount; (void)objectives; // TODO
    return false;
}

bool handleUseCommand(char itemSymbol, int currentFloor, int currentRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES]) {
    (void)itemSymbol; (void)currentFloor; (void)currentRoom; (void)energy; (void)time;
    (void)score; (void)inventory; (void)inventoryCount; (void)objectives; // TODO
    return false;
}

bool handleUnlockCabinetCommand(const char* code, int currentFloor, int currentRoom,
                                bool objectives[NUM_OBJECTIVES], int& score,
                                char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                                int& entityCount) {
    (void)code; (void)currentFloor; (void)currentRoom; (void)objectives;
    (void)score; (void)roomEntities; (void)entityCount; // TODO
    return false;
}

bool checkEndConditions(int energy, int time, const bool objectives[NUM_OBJECTIVES]) {
    if (energy <= 0 || time <= 0) {
        return true;
    }
    if (objectives[OBJ_MISSION_WON]) {
        return true;
    }
    return false;
}
