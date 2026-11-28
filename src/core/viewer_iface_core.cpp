#include "viewer_iface.h"
#include "util.h"
#include "level.h"
#include <iostream>
using namespace std;

const char* LINKS[NUM_FLOORS][ROOMS_PER_FLOOR - 1] = {
    {"--W--", "[L1]-", "--D--"},
    {"--D--", "[L1]-", "--D--"},
    {"--L--", "--W--", "--D--"},
    {"--W--", " ### ", "--D--"},
    {"--D--", "--D--", "--W--"}
};
const char* VERTICAL[NUM_FLOORS] = {
    "   S1 (R1)  L1 (R2/R3)",
    "   S1 S2 (R1)  L1 (R2/R3)  L2 (R4)",
    "   S2 (R1)",
    "   S3 (R1)  L2 (R4)",
    "   S3 (R1)  L2 (R4)"
};
const char* ROOM_NAMES[NUM_FLOORS][ROOMS_PER_FLOOR] = {
    {"Entrance Lobby", "Reception", "Waiting Area", "Utility Room"},
    {"Lounge", "Archive", "Lab", "Meeting Room"},
    {"Stairwell Annex", "Security Vestibule", "Animal Room", "Control Room"},
    {"First Aid Station", "Medical Room", "Maintenance Corridor", "Office"},
    {"Security Room", "Train Boarding Platform", "Storage Room", "Observation Deck"}
};
const char* OBJECTIVE_LABELS[OBJ_MISSION_WON] = {
    "Collect the Master Key (F1-R2)",
    "Find clue digit 4 (F3-R2)",
    "Find clue digit 2 (F4-R3)",
    "Find clue digit 7 (F5-R1)",
    "Unlock the cabinet with the code (F5-R1)",
    "Get the Train Key (F5-R1)",
    "Collect the Fuse (F3-R2)",
    "Install the Fuse at the Control Panel (F3-R4)",
    "Collect the Lift Pass (F3-R4)",
    "Restore Lift 2",
    "Rescue the passenger (F4-R1)",
    "Collect the Cat Carrier (F5-R3)",
    "Secure the cat (F3-R3)",
    "Prepare and board the train (F5-R2)"
};
const char* getRoomName(int floor, int room){
    if (floor < 1 || floor > NUM_FLOORS || room < 1 || room > ROOMS_PER_FLOOR){
        return "Unknown";
    }
    return ROOM_NAMES[floor-1][room-1];
}
void printBar(int value, int maxValue){
    int width = 20;
    int filled = 0;
    if (maxValue > 0){
        filled = clampInt(value, 0, maxValue) * width / maxValue;
    }
    cout << "[";
    for (int i = 0; i < width; i++){
        if (i < filled){
            cout << "#";
        } else {
            cout << ".";
        }
    }
    cout << "] " << value << "/" << maxValue;
}
void printRoomCell(int floor, int room, int playerFloor, int playerRoom){
    if (floor == playerFloor && room == playerRoom){
        cout << "[>F" << floor << "-R" << room << "<]";
    } else {
        cout << "[ F" << floor << "-R" << room << " ]";
    }
}
void printCheck(bool done, const char* label){
    if (done){
        cout << " [X] ";
    } else {
        cout << " [ ] ";
    }
    cout << label << endl;
}
void renderDashboard(int playerFloor, int playerRoom, int energy, int time, int score,
                     const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                     int emergencyTripsLeft) {
    printHeader("STATUS");
    cout << "Location : F" << playerFloor << "-R" << playerRoom << " (" << getRoomName(playerFloor, playerRoom) << ")" << endl;
    cout << "Energy   : ";
    printBar(energy, MAX_ENERGY);
    cout << endl;
    cout << "Time     : ";
    printBar(time, MAX_TIME);
    cout << endl;
    cout << "Score    : " << score << " / 520" << endl;
    cout << "Lift 2 emergency trips : " << emergencyTripsLeft << endl;
    cout << "Inventory: ";
    if (inventoryCount == 0) {
        cout << "Empty";
    } else {
        for (int i = 0; i < inventoryCount; i++) {
            cout << getEntityName(inventory[i]);
            if (i < inventoryCount - 1) {
                cout << ", ";
            }
        }
    }
    cout << endl;
}
void renderRoomDetails(int floor, int room,
                       const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                       int entityCount) {
    if (floor < 1 || floor > NUM_FLOORS || room < 1 || room > ROOMS_PER_FLOOR) {
        return;
    }
    cout << "Room: F" << floor << "-R" << room << " - " << getRoomName(floor, room) << endl;
    if (entityCount == 0) {
        cout << "Nothing here." << endl;
        return;
    }
    for (int i = 0; i < entityCount; i++) {
        char sym = roomEntities[floor-1][room-1][i];
        cout << "  [" << sym << "] " << getEntityName(sym) << endl;
    }
}
void renderAsciiBuilding(int playerFloor, int playerRoom) {
    cout<<"BUILDING MAP" << endl;
    cout<<endl;
    for (int floor = NUM_FLOORS; floor >= 1; floor--) {
        for (int room = 1; room <= ROOMS_PER_FLOOR; room++) {
            printRoomCell(floor, room, playerFloor, playerRoom);
            if (room < ROOMS_PER_FLOOR) {
                cout << LINKS[floor-1][room-1];
            }
        }
        cout << VERTICAL[floor-1] << endl;
        cout << endl;
    }
    cout << "Legend: [>Fx-Rx<] = You, D = Door, W = Window, L = Locked, ### = Wall"<<endl;
    cout << "        S1-S3 = Stairs, L1/L2 = Lifts" << endl;
}
void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES]) {
    printHeader("OBJECTIVES");
    int progress = 0;
    for (int i = 0; i < OBJ_MISSION_WON; i++) {
        printCheck(objectives[i], OBJECTIVE_LABELS[i]);
        if (objectives[i]) {
            progress++;
        }
    }
    cout << "Progress: " << progress << " / " << OBJ_MISSION_WON << endl;
}