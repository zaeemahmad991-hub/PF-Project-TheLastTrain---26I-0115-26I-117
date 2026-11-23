#include "level.h"
#include "util.h"
#include "physics.h"
# include <iostream>
# include <fstream>
# include <cstring>
# include <cstdlib>
using namespace std;
bool loadBuildingMap(const char* filename,
                     char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                     char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                     int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR]) {
    ifstream file(filename);
    if (!file)
        return false;
    for(int i=0;i<NUM_FLOORS;i++){
        for(int j=0;j<ROOMS_PER_FLOOR;j++){
            if (j!=ROOMS_PER_FLOOR-1){
                horizontalConn[i][j]=CONN_NONE;
            }
            entityCounts[i][j]=0;
            for (int k=0;k<MAX_ENTITIES_PER_ROOM;k++){
                roomEntities[i][j][k]=ENT_NONE;
            }
        }
    }
    char line[MAX_LINE_LEN];
    int floorsRead=0;
    while(floorsRead<NUM_FLOORS && file.getline(line, MAX_LINE_LEN)){
        char* start = strstr(line,"-R1");
        if (start != nullptr){
            if(start-line<2){
                continue;
            }
            int f = *(start -1) - '0';
            if(f<1||f>NUM_FLOORS){
                continue;
            }
            floorsRead++;
            int col[ROOMS_PER_FLOOR];
            col[0]=(start-2)-line;
            int r =0;
            for(char* p=start;*p!='\0' && r<ROOMS_PER_FLOOR-1;p++){
                if(*p == 'F'){
                    col[r+1]=p-line;
                    if (*(p-2)=='1'){
                        r++;
                        continue;
                    }
                    horizontalConn[f-1][r] = *(p-2);
                    r++;
                }
            }
            if(file.getline(line,MAX_LINE_LEN)){
                int len=strlen(line);
                for(int i =0;i<ROOMS_PER_FLOOR;i++){
                    for(int itemSearch = col[i]-1;itemSearch<=col[i]+9 && itemSearch<len;itemSearch++){
                        if (line[itemSearch]!=' ' && entityCounts[f-1][i]<MAX_ENTITIES_PER_ROOM){
                            roomEntities[f-1][i][entityCounts[f-1][i]]=line[itemSearch];
                            entityCounts[f-1][i]++;
                        }
                    }
                }
            }
        }
    }
    file.close();
    return true;
}
bool loadInitialState(const char* filename,
                      int& playerFloor, int& playerRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft) {
    ifstream file(filename);
    if (!file)
        return false;
    playerFloor=0;
    playerRoom=0;
    energy=INITIAL_ENERGY;
    time=INITIAL_TIME;
    score=INITIAL_SCORE;
    inventoryCount=0;
    for(int i=0;i<MAX_INVENTORY_CAPACITY;i++){
        inventory[i]=ENT_NONE;
    }
    for(int i=0;i<NUM_OBJECTIVES;i++){
        objectives[i]=false;
    }
    emergencyTripsLeft=0;
    char line[MAX_LINE_LEN];
    while(file.getline(line, MAX_LINE_LEN)){
        char* colon=strchr(line,':');
        if (colon != nullptr){
            *colon='\0';
            char* value=colon+1;
            trimWhitespace(line);
            trimWhitespace(value);
            if (stringEquals(line,"INITIAL_ENERGY")){
                energy=clampInt(atoi(value),0,MAX_ENERGY);
            }
            else if (stringEquals(line,"INITIAL_TIME")){
                time=clampInt(atoi(value),0,MAX_TIME);
            }
            else if (stringEquals(line,"INITIAL_SCORE")){
                score=atoi(value);
                if (score<0){
                    score=0;
                }
            }
        }
    }
    file.close();
    return true;
}
bool validateStartingRoom(int startFloor, int startRoom,
                          const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1]) {
    if(startFloor<1||startFloor>NUM_FLOORS){
        return false;
    }
    if(startRoom<1||startRoom>ROOMS_PER_FLOOR){
        return false;
    }
    bool visited[NUM_FLOORS][ROOMS_PER_FLOOR];
    for(int i=0;i<NUM_FLOORS;i++){
        for(int j=0;j<ROOMS_PER_FLOOR;j++){
            visited[i][j]=false;
        }
    }
    int queueFloor[TOTAL_ROOMS];
    int queueRoom[TOTAL_ROOMS];
    int front=0;
    int back=0;
    queueFloor[back]=startFloor;
    queueRoom[back]=startRoom;
    back++;
    visited[startFloor-1][startRoom-1]=true;
    while(front<back){
        int f=queueFloor[front];
        int r=queueRoom[front];
        front++;
        for(int tf=1;tf<=NUM_FLOORS;tf++){
            for(int tr=1;tr<=ROOMS_PER_FLOOR;tr++){
                char connType=CONN_NONE;
                if(!visited[tf-1][tr-1] && isDirectlyConnected(f,r,tf,tr,horizontalConn,connType)){
                    visited[tf-1][tr-1]=true;
                    queueFloor[back]=tf;
                    queueRoom[back]=tr;
                    back++;
                }
            }
        }
    }
    return visited[4][1];
}
const char* getEntityName(char entitySymbol) {
    if(entitySymbol==ENT_MASTER_KEY){
        return "Master Key";
    }
    else if(entitySymbol==ENT_ENERGY_PACK){
        return "Energy Pack";
    }
    else if(entitySymbol==ENT_EMERGENCY_TOOLKIT){
        return "Emergency Toolkit";
    }
    else if(entitySymbol==ENT_CLUE_4){
        return "Clue Digit 4";
    }
    else if(entitySymbol==ENT_FUSE){
        return "Fuse";
    }
    else if(entitySymbol==ENT_CAT){
        return "Cat";
    }
    else if(entitySymbol==ENT_CONTROL_PANEL){
        return "Control Panel";
    }
    else if(entitySymbol==ENT_LIFT_PASS){
        return "Lift Pass";
    }
    else if(entitySymbol==ENT_PASSENGER){
        return "Injured Passenger";
    }
    else if(entitySymbol==ENT_MEDICAL_KIT){
        return "Medical Kit";
    }
    else if(entitySymbol==ENT_EMERGENCY_CARD){
        return "Emergency Access Card";
    }
    else if(entitySymbol==ENT_CLUE_2){
        return "Clue Digit 2";
    }
    else if(entitySymbol==ENT_CLUE_7){
        return "Clue Digit 7";
    }
    else if(entitySymbol==ENT_CABINET_TRAIN_KEY){
        return "Cabinet / Train Key";
    }
    else if(entitySymbol==ENT_CAT_CARRIER){
        return "Cat Carrier";
    }
    else if(entitySymbol==ENT_OBSERVATION_TOKEN){
        return "Observation Token";
    }
    return "Unknown";
}
