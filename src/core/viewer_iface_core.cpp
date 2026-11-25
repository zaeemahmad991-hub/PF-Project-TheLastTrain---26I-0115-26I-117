#include "viewer_iface.h"
#include "util.h"

void renderDashboard(int playerFloor, int playerRoom, int energy, int time, int score,
                     const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                     int emergencyTripsLeft) {
    (void)playerFloor; (void)playerRoom; (void)energy; (void)time; (void)score;
    (void)inventory; (void)inventoryCount; (void)emergencyTripsLeft; // TODO
}

void renderRoomDetails(int floor, int room,
                       const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                       int entityCount) {
    (void)floor; (void)room; (void)roomEntities; (void)entityCount; // TODO
}

void renderAsciiBuilding(int playerFloor, int playerRoom) {
    (void)playerFloor; (void)playerRoom;  // TODO
}

void renderObjectivesChecklist(const bool objectives[NUM_OBJECTIVES]) {
    (void)objectives; // TODO
}
