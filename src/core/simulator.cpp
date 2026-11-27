#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"
# include "level.h"
# include <iostream>
# include <cstring>
# include <cstdlib>
using namespace std;

void removeFromFloor(char shelf[], int &count, char sym){
    for (int i = 0; i < count; i++){
        if (shelf[i] == sym){
            for (int j = i; j < count - 1; j++){
                shelf[j] = shelf[j + 1];
            }
            shelf[count - 1] = ENT_NONE;
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
    
    // TODO
    std::cout << "[stub] runSimulation not implemented yet\n";
}

bool handleMoveCommand(int targetFloor, int targetRoom,
                       int& currentFloor, int& currentRoom,
                       int& energy, int& time,
                       const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                       const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                       int& emergencyTripsLeft, bool lift2Operational) {
    if (targetFloor < 1 || targetFloor > NUM_FLOORS || targetRoom < 1 || targetRoom > ROOMS_PER_FLOOR) {
        cout << "No such Room!" << endl;
        return false;
    }
    char CONN_TYPE = CONN_NONE;
    if (!isDirectlyConnected(currentFloor, currentRoom, targetFloor, targetRoom, horizontalConn, CONN_TYPE)) {
        cout << "Rooms are not directly connected!" << endl;
        return false;
    }
    if (!canTraverse(targetFloor, targetRoom, CONN_TYPE, inventory, inventoryCount, emergencyTripsLeft, lift2Operational)) {
        return false;
    }
    int DeltaE = 0, DeltaT = 0;
    if (CONN_TYPE == CONN_STAIRS || CONN_TYPE == CONN_DOOR || CONN_TYPE == CONN_LOCKED_DOOR){
        DeltaE = COST_MOVE_ORDINARY_ENERGY;
        DeltaT = COST_MOVE_ORDINARY_TIME;
    } else if (CONN_TYPE == CONN_WINDOW){
        DeltaE = COST_MOVE_WINDOW_ENERGY;
        DeltaT = COST_MOVE_WINDOW_TIME;
    } else if (CONN_TYPE == CONN_LIFT1 || CONN_TYPE == CONN_LIFT2){
        DeltaE = COST_MOVE_LIFT_ENERGY;
        DeltaT = COST_MOVE_LIFT_TIME;
    }
    applyResourceDelta(energy, time, DeltaE, DeltaT);
    if (CONN_TYPE == CONN_LIFT2 && !lift2Operational) {
        emergencyTripsLeft--;
    }
    currentFloor = targetFloor;
    currentRoom = targetRoom;
    cout << "Moved to F" << currentFloor << "-R" << currentRoom << "." << endl;
    return true;
}

bool handleInspectCommand(int currentFloor, int currentRoom,
                          int& energy, int& time,
                          const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int entityCount,
                          bool objectives[NUM_OBJECTIVES], int& score) {
    if (currentFloor < 1 || currentFloor > NUM_FLOORS || currentRoom < 1 || currentRoom > ROOMS_PER_FLOOR) {
        return false;
    }
    applyResourceDelta(energy, time, COST_INSPECT_ENERGY, COST_INSPECT_TIME);
    for(int i=0;i<entityCount;i++){
        char item = roomEntities[currentFloor-1][currentRoom-1][i];
        cout<<"  [" << item << "] " << getEntityName(item) <<endl;
        if (item == ENT_CLUE_2 && !objectives[OBJ_CLUE_2]) {
            objectives[OBJ_CLUE_2] = true;
            applyScoreDelta(score, SCORE_CLUE_FOUND);
            cout<<"You discovered clue digit 2!"<<endl;
        } else if (item == ENT_CLUE_4 && !objectives[OBJ_CLUE_4]) {
            objectives[OBJ_CLUE_4] = true;
            applyScoreDelta(score, SCORE_CLUE_FOUND);
            cout<<"You discovered clue digit 4!"<<endl;
        } else if (item == ENT_CLUE_7 && !objectives[OBJ_CLUE_7]) {
            objectives[OBJ_CLUE_7] = true;
            applyScoreDelta(score, SCORE_CLUE_FOUND);
            cout<<"You discovered clue digit 7!"<<endl;
        }
    }
    if (entityCount == 0){
        cout<<"Nothing useful here."<<endl;
    }
    return true;
}

bool handleCollectCommand(char itemSymbol, int currentFloor, int currentRoom,
                          int& energy, int& time, int& score,
                          char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int& entityCount,
                          char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                          bool objectives[NUM_OBJECTIVES]) {
    if (currentFloor < 1 || currentFloor > NUM_FLOORS || currentRoom < 1 || currentRoom > ROOMS_PER_FLOOR) {
        return false;
    }
    char* shelf = roomEntities[currentFloor-1][currentRoom-1];
    bool found = false;
    for (int i = 0; i < entityCount; i++){
        if (shelf[i] == itemSymbol){
            found = true;
            break;
        }
    }
    if (!found){
        cout << "That item is not here." << endl;
        return false;
    }
    if (itemSymbol == ENT_CAT){
        if (objectives[OBJ_CAT_SECURED]){
            cout << "The cat is already secured." << endl;
        } else if (hasItem(inventory, inventoryCount, ENT_CAT_CARRIER)){
            cout << "Use the Cat Carrier here: use @" << endl;
        } else {
            cout << "You need the Cat Carrier to secure the cat." << endl;
        }
        return false;
    }
    if (itemSymbol == ENT_PASSENGER){
        if (objectives[OBJ_PASSENGER_RESCUED]){
            cout << "The passenger is already rescued." << endl;
        } else if (hasItem(inventory, inventoryCount, ENT_MEDICAL_KIT)){
            cout << "Treat the passenger with the kit: use %" << endl;
        } else {
            cout << "You need the Medical Kit to rescue the passenger." << endl;
        }
        return false;
    }
    if (itemSymbol == ENT_CONTROL_PANEL){
        cout << "You cannot collect the Control Panel. It is fixed in place." << endl;
        return false;
    }
    if (itemSymbol == ENT_CABINET_TRAIN_KEY){
        cout << "The cabinet is locked. Type: unlock <code>" << endl;
        return false;
    }
    if (itemSymbol == ENT_CLUE_2 || itemSymbol == ENT_CLUE_4 || itemSymbol == ENT_CLUE_7){
        cout << "You cannot collect clues. Use inspect to study them." << endl;
        return false;
    }
    if (itemSymbol == ENT_ENERGY_PACK || itemSymbol == ENT_EMERGENCY_TOOLKIT || itemSymbol == ENT_OBSERVATION_TOKEN){
        removeFromFloor(shelf, entityCount, itemSymbol);
        applyResourceDelta(energy, time, COST_ITEM_PICKUP_ENERGY, COST_ITEM_PICKUP_TIME);
        if (itemSymbol == ENT_ENERGY_PACK){
            applyResourceDelta(energy, time, BONUS_ENERGY_PACK, 0);
            cout << "Energy Pack used: +25 Energy!" << endl;
        } else if (itemSymbol == ENT_EMERGENCY_TOOLKIT){
            applyResourceDelta(energy, time, BONUS_EMERGENCY_TOOLKIT, 0);
            cout << "Emergency Toolkit used: +15 Energy!" << endl;
        } else {
            applyResourceDelta(energy, time, 0, BONUS_OBSERVATION_TOKEN);
            cout << "Observation Token used: +15 Time!" << endl;
        }
        return true;
    }
    if (!addItem(inventory, inventoryCount, itemSymbol)){
        cout << "Your inventory is full. Cannot collect more items." << endl;
        return false;
    }
    removeFromFloor(shelf, entityCount, itemSymbol);
    applyResourceDelta(energy, time, COST_ITEM_PICKUP_ENERGY, COST_ITEM_PICKUP_TIME);
    if (itemSymbol == ENT_MASTER_KEY){
        objectives[OBJ_MASTER_KEY] = true;
        applyScoreDelta(score, SCORE_KEY_COLLECT);
    } else if (itemSymbol == ENT_FUSE){
        objectives[OBJ_FUSE_COLLECTED] = true;
    } else if (itemSymbol == ENT_LIFT_PASS){
        objectives[OBJ_LIFT_PASS] = true;
        applyScoreDelta(score, SCORE_LIFT_PASS);
        if (objectives[OBJ_FUSE_INSTALLED]){
            objectives[OBJ_LIFT2_ACTIVE] = true;
            cout << "Lift 2 is now fully operational!" << endl;
        }
    } else if (itemSymbol == ENT_CAT_CARRIER){
        objectives[OBJ_CAT_CARRIER] = true;
    }
    cout << "Collected: " << getEntityName(itemSymbol) << endl;
    return true;
}

bool handleUseCommand(char itemSymbol, int currentFloor, int currentRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES]) {
    if (itemSymbol == ENT_CABINET_TRAIN_KEY){
        if (!objectives[OBJ_TRAIN_KEY]){
            cout << "You don't have the train key." << endl;
            return false;
        }
    } else if (!hasItem(inventory, inventoryCount, itemSymbol)){
        cout << "You don't have that item in your inventory." << endl;
        return false;
    }
    bool done = false;
    if (itemSymbol == ENT_FUSE && currentFloor == 3 && currentRoom == 4){
        done = installFuseControlPanel(inventory, inventoryCount, objectives, score);

    }
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
