#include "Constants.h"
#include "Characters.h"
#include "DungeonGenerator.h"

class Camera{
    private:
    int row;
    int column;
    int range = 3;
    Character target;
    public:
    ~Camera(){}
    Camera(){}
    Camera(Character target,int FOV){
        this->target = target;
        this->range = FOV;
        column = target.getColumn();
        row = target.getRow();
    }
    void setPosition(int row, int column){
        this->row = row;
        this->column = column;
        if(column - range < 0) column = range;
        if(row - range < 0) row = range;
        if(row + range >= MaxFil) row = MaxFil-range-1;
        if(column + range >= MaxCol) column = MaxCol-range-1;
    }
    void updatePosition(){
        column = target.getColumn();
        row = target.getRow();
        if((column - range) < 0) column = range;
        if((row - range) < 0) row = range;
        if((row + range) >= MaxFil) row = MaxFil-range-1;
        if((column + range) >= MaxCol) column = MaxCol-range-1;
    }
    int getRow(){return row;}
    int getColumn(){return column;}
    void printFrame(){
        HANDLE hcon;
        CHAR_INFO chiBuffer[49]; //[(range*2+1)*(range*2+1)]
        COORD bufCoord = {0,0}; 
        COORD size = {(SHORT)(range*2+1),(SHORT)(range*2+1)};
        SMALL_RECT writeRect = {0,0,(SHORT)(range*2+1),(SHORT)(range*2+1)};
        hcon = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hcon,bufCoord);
        int c = 0;
        for(int i = 0;i < range*2+1;i++){
            for(int j = 0;j < range*2+1;j++){
                chiBuffer[c].Char.AsciiChar = mazmorra[row-range+i][column-range+j];
                chiBuffer[c++].Attributes = FOREGROUND_INTENSITY;
            }
        }
        WriteConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE),chiBuffer,size,bufCoord,&writeRect);
    }
};