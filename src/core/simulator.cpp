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
    if (currentFloor < 1 || currentFloor > NUM_FLOORS || currentRoom < 1 || currentRoom > ROOMS_PER_FLOOR) {
        return false;
    }
    applyResourceDelta(energy, time, COST_INSPECT_ENERGY, COST_INSPECT_TIME);
    for(int i=0;i<entityCount-1;i++){
        char item = roomEntities[currentFloor-1][currentRoom-1][i];
        cout<<"  [" << item << "] " << getEntityName(item) <<endl;
        if (item == ENT_CLUE_2 && !objectives[OBJ_CLUE_2]) {
            objectives[OBJ_CLUE_2_FOUND] = true;
            applyScoreDelta(score, SCORE_CLUE_FOUND);
            cout<<"You discovered clue digit 2!"<<endl;
        } else if (item == ENT_CLUE_4 && !objectives[OBJ_CLUE_4]) {
            objectives[OBJ_CLUE_4_FOUND] = true;
            applyScoreDelta(score, SCORE_CLUE_FOUND);
            cout<<"You discovered clue digit 4!"<<endl;
        } else if (item == ENT_CLUE_7 && !objectives[OBJ_CLUE_7]) {
            objectives[OBJ_CLUE_7_FOUND] = true;
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
    for (int i = 0; i < entityCount; i++){
        if (shelf[i] == itemSymbol){
            if (itemSymbol == ENT_CAT){
                if (objectives[OBJ_CAT_SECURED]){
                    cout << "The cat is already secured." << endl;
                    return false;
                } else if (hasItem(inventory, inventoryCount, ENT_CAT_CARRIER)){
                    cout << "Use the Cat Carrier here: use @." << endl;
                    return false;
                } else {
                    cout << "You need the Cat Carrier to secure the cat." << endl;
                    return false;
                }
            } else if (itemSymbol == ENT_PASSENGER){
                if (objectives[OBJ_PASSENGER_RESCUED]){
                    cout << "The passenger is already rescued." << endl;
                    return false;
                } else if (hasItem(inventory, inventoryCount, ENT_MEDICAL_KIT)){
                    cout << "Treat the passenger with the kit: use %." << endl;
                    return false;
                } else {
                    cout << "You need the Medical Kit to rescue the passenger." << endl;
                    return false;
                }
            } else if (itemSymbol == ENT_CONTROL_PANEL){
                cout << "You cannot collect the Control Panel. It is fixed in place." << endl;
                return false;
            } else if (itemSymbol == ENT_CABINET_TRAIN_KEY){
                cout << "The Cabinet is locked. Type Code to open" << endl;
                return false; 
            } else if (itemSymbol == ENT_CLUE_2 || itemSymbol == ENT_CLUE_4 || itemSymbol == ENT_CLUE_7){
                cout << "You cannot collect clues. You can only inspect them." << endl;
                return false;
            } else if (itemSymbol == ENT_ENERGY_PACK){
                removeFromFloor(shelf, entityCount, ENT_ENERGY_PACK);
                applyResourceDelta(energy, time, COST_ITEM_PICKUP_ENERGY, COST_ITEM_PICKUP_TIME);
                applyResourceDelta(energy, time, BONUS_ENERGY_PACK, 0);
                cout<< "+25 energy!" << endl;
                return true;
            } else if (itemSymbol == ENT_EMERGENCY_TOOLKIT){
                removeFromFloor(shelf, entityCount, ENT_EMERGENCY_TOOLKIT);
                applyResourceDelta(energy, time, COST_ITEM_PICKUP_ENERGY, COST_ITEM_PICKUP_TIME);
                applyResourceDelta(energy, time, BONUS_EMERGENCY_TOOLKIT, 0);
                cout<< "+15 energy!" << endl;
                return true;
            } else if (itemSymbol == ENT_OBSERVATION_TOKEN){
                removeFromFloor(shelf, entityCount, ENT_OBSERVATION_TOKEN);
                applyResourceDelta(energy, time, COST_ITEM_PICKUP_ENERGY, COST_ITEM_PICKUP_TIME);
                applyResourceDelta(energy, time, 0 , BONUS_OBSERVATION_TOKEN);
                cout<< "+15 time!" << endl;
                return true;
            } else {
                if (!addItem(inventory, inventoryCount, itemSymbol)){
                    cout << "Your inventory is full. Cannot collect more items." << endl;
                    return false;
                }
                removeFromFloor(shelf, entityCount, itemSymbol);
                applyResourceDelta(energy, time, COST_ITEM_PICKUP_ENERGY, COST_ITEM_PICKUP_TIME);
                if (itemSymbol == ENT_MASTER_KEY){
                    objectives[OBJ_MASTER_KEY] = true;
                    applyScoreDelta(score, SCORE_KEY_COLLECT);
                    cout<<"Collected: "<< getEntityName(itemSymbol) << endl;
                    return true;
                }
                if (itemSymbol == ENT_FUSE){
                    objectives[OBJ_FUSE_COLLECTED] = true;
                    cout<<"Collected: "<< getEntityName(itemSymbol) << endl;
                    return true;
                }
                if (itemSymbol == ENT_LIFT_PASS){
                    objectives[OBJ_LIFT_PASS] = true;
                    applyScoreDelta(score, SCORE_LIFT_PASS);
                    cout<<"Collected: "<< getEntityName(itemSymbol) << endl;
                    if (objectives[OBJ_FUSE_INSTALLED]){
                        objectives[OBJ_LIFT2_ACTIVE] = true;
                    }
                    return true;
                }
                if (itemSymbol == ENT_CAT_CARRIER){
                    objectives[OBJ_CAT_CARRIER] = true;
                    cout<<"Collected: "<< getEntityName(itemSymbol) << endl;
                    return true;
                }
                if (itemSymbol == ENT_MEDICAL_KIT || itemSymbol == ENT_EMERGENCY_CARD)  {
                    cout<<"Collected: "<< getEntityName(itemSymbol) << endl;
                    return true;
                }
                    
            }
    }
    cout << "That item is not here." << endl;
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
