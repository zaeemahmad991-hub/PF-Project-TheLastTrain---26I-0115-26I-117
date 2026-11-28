#include "logger.h"
#include "util.h"
#include "state.h"
#include "physics.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
using namespace std;

bool saveGameStateToFile(const char* filename,
                         int playerFloor, int playerRoom,
                         int energy, int time, int score,
                         const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                         const bool objectives[NUM_OBJECTIVES], int emergencyTripsLeft,
                         const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                         const int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]){
    ofstream file(filename);
    if(!file){
        return false;
    }                
    file << "PLAYER_FLOOR: " << playerFloor << endl;
    file << "PLAYER_ROOM: " << playerRoom << endl;
    file << "ENERGY: " << energy << endl;
    file << "TIME: " << time << endl;
    file << "SCORE: " << score << endl;
    file << "EMERGENCY_TRIPS: " << emergencyTripsLeft << endl;
    file << "INVENTORY: ";
    if (inventoryCount == 0) {
        file << "NONE" << endl;
    } else {
        for (int i = 0; i < inventoryCount; i++) {
            file << inventory[i];
        }
        file << endl;
    }
    file << "OBJECTIVES: ";
    for (int i = 0; i < NUM_OBJECTIVES; i++) {
        file << (objectives[i] ? 1 : 0);
    }
    file << endl;
    for (int f = 0; f < NUM_FLOORS; f++) {
        for (int r = 0; r < ROOMS_PER_FLOOR; r++) {
            file << "ROOM_" << (f + 1) << "_" << (r + 1) << ": ";
            if (entityCounts[f][r] == 0) {
                file << "NONE";
            } else {
                for (int e = 0; e < entityCounts[f][r]; e++) {
                    file << roomEntities[f][r][e];
                }
            }
            file << endl;
        }
    }
    file.close();
    return true;
}

bool loadGameStateFromFile(const char* filename,
                           int& playerFloor, int& playerRoom,
                           int& energy, int& time, int& score,
                           char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                           bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                           char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                           int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]){
    ifstream file(filename);
    if(!file){
        return false;
    }
    inventoryCount = 0;
    for (int i = 0; i < MAX_INVENTORY_CAPACITY; i++) {
        inventory[i] = ENT_NONE;
    }
    for (int i = 0; i < NUM_OBJECTIVES; i++) {
        objectives[i] = false;
    }
    for (int f = 0; f < NUM_FLOORS; f++) {
        for (int r = 0; r < ROOMS_PER_FLOOR; r++) {
            entityCounts[f][r] = 0;
            for (int e = 0; e < MAX_ENTITIES_PER_ROOM; e++) {
                roomEntities[f][r][e] = ENT_NONE;
            }
        }
    }
    bool gotFloor = false, gotRoom = false, gotObjectives = false;
    char line[MAX_LINE_LEN];
    while(file.getline(line, MAX_LINE_LEN)){
        char* colon=strchr(line,':');
        if (colon != nullptr){
            *colon='\0';
            char* value=colon+1;
            trimWhitespace(line);
            trimWhitespace(value);
            if (stringEquals(line,"PLAYER_FLOOR")){
                int v=atoi(value);
                if (v >= 1 && v <= NUM_FLOORS) {
                    playerFloor = v;
                    gotFloor = true;
                } else {
                    file.close();
                    return false;
                }
            } else if (stringEquals(line,"PLAYER_ROOM")){
                int v=atoi(value);
                if (v >= 1 && v <= ROOMS_PER_FLOOR) {
                    playerRoom = v;
                    gotRoom = true;
                } else {
                    file.close();
                    return false;
                }
            } else if (stringEquals(line,"ENERGY")){
                energy=clampInt(atoi(value),0,MAX_ENERGY);
            } else if (stringEquals(line,"TIME")){
                time=clampInt(atoi(value),0,MAX_TIME);
            } else if (stringEquals(line,"SCORE")){
                score=atoi(value);
                if (score<0){
                    score=0;
                }
            } else if (stringEquals(line,"EMERGENCY_TRIPS")){
                emergencyTripsLeft=clampInt(atoi(value),0,3);
            } else if (stringEquals(line,"INVENTORY")){
                if (!stringEquals(value, "NONE")) {
                    for (char* i = value; *i != '\0'; i++) {
                        addItem(inventory, inventoryCount, *i);
                    }
                }
            } else if (stringEquals(line,"OBJECTIVES")){
                if(strlen(value) < NUM_OBJECTIVES) {
                    file.close();
                    return false;
                }
                for (int i = 0; i < NUM_OBJECTIVES; i++) {
                    objectives[i] = (value[i] == '1');
                }
                gotObjectives = true;
            } else if (strncmp(line, "ROOM_", 5) == 0) {
                int f = line[5] - '0';
                int r = line[7] - '0';
                if (f < 1 || f > NUM_FLOORS || r < 1 || r > ROOMS_PER_FLOOR) {
                    continue;
                }
                if (!stringEquals(value, "NONE")) {
                    for (int i = 0; value[i] != '\0' && entityCounts[f-1][r-1] < MAX_ENTITIES_PER_ROOM; i++) {
                        roomEntities[f-1][r-1][entityCounts[f-1][r-1]] = value[i];
                        entityCounts[f-1][r-1]++;
                    }
                }
            }
        }
    }
    file.close();
    if (!gotFloor || !gotRoom || !gotObjectives) {
        return false;
    }
    return true;
}

void writeGameSummaryReport(const char* filename,
                            int score, int remainingEnergy, int remainingTime,
                            const bool objectives[NUM_OBJECTIVES], bool victory) {
    ofstream file(filename);
    if (!file){
        cout << "Could not write summary to " << filename << endl;
        return;
    }
    const char* labels[OBJ_MISSION_WON] = {
        "Master Key collected",
        "Clue digit 4 found",
        "Clue digit 2 found",
        "Clue digit 7 found",
        "Cabinet unlocked with code 427",
        "Train Key acquired",
        "Fuse collected",
        "Fuse installed at Control Panel",
        "Lift Pass collected",
        "Lift 2 restored",
        "Passenger rescued",
        "Cat Carrier collected",
        "Cat secured",
        "Train prepared and boarded"
    };
    file << "===========================================" << endl;
    file << " THE LAST TRAIN - GAME SUMMARY" << endl;
    file << "===========================================" << endl;
    if (victory){
        file << "Outcome          : VICTORY" << endl;
    } else {
        file << "Outcome          : DEFEAT" << endl;
    }
    file << "Final Score      : " << score << " / 520" << endl;
    file << "Energy Remaining : " << remainingEnergy << " / " << MAX_ENERGY << endl;
    file << "Time Remaining   : " << remainingTime << " / " << MAX_TIME << endl;
    file << "-------------------------------------------" << endl;
    file << " OBJECTIVES" << endl;
    file << "-------------------------------------------" << endl;
    int completed = 0;
    for (int i = 0; i < OBJ_MISSION_WON; i++){
        if (objectives[i]){
            file << " [X] ";
            completed++;
        } else {
            file << " [ ] ";
        }
        file << labels[i] << endl;
    }
    file << "-------------------------------------------" << endl;
    file << " SESSION AUDIT" << endl;
    file << "-------------------------------------------" << endl;
    file << "Objectives completed : " << completed << " / " << OBJ_MISSION_WON << endl;   
    if (victory){
        file << "Result               : Mission completed." << endl;
    } else if (remainingEnergy <= 0){
        file << "Result               : Energy depleted." << endl;
    } else if (remainingTime <= 0){
        file << "Result               : Time depleted." << endl;
    } else {
        file << "Result               : Player quit before finishing." << endl;
    }
    file << "===========================================" << endl;
    file.close();
}
