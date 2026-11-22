#include "util.h"
#include <iostream>
#include <cstring>
using namespace std;
int clampInt(int val, int minVal, int maxVal){
    if(val<minVal){
        return minVal;
    }
    if(val>maxVal){
        return maxVal;
    }
    return val;
}
bool isSpaceChar(char c){
    return c==' '||c=='\t'||c=='\r'||c=='\n';
}
void trimWhitespace(char* str){
    if(str==nullptr){
        return;
    }
    int len=strlen(str);
    while(len>0&&isSpaceChar(str[len-1])){
        str[len-1]='\0';
        len--;
    }
    int start=0;
    while(start<len&&isSpaceChar(str[start])){
        start++;
    }
    if(start>0){
        for(int i=0;i<=len-start;i++){
            str[i]=str[i+start];
        }
    }
}
bool stringEquals(const char* a, const char* b){
    if(a==nullptr||b==nullptr){
        return false;
    }
    return strcmp(a,b)==0;
}
void printHeader(const char* title){
    cout<<"==========================================="<<endl;
    cout<<" "<<title<<endl;
    cout<<"==========================================="<<endl;
}
void clearScreen(){
    cout<<"\033[2J\033[H"<<flush;
}
