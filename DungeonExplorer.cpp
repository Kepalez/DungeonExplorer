#include "Constants.h"
#include "Camera.h"

const int QantMonsters = 7;

void iniciarJuego(){

    generarDungeon();
    Hero jugador("Kevin",50,15);
    jugador.setPosition((MaxFil+tamanoSalas)/2,(MaxCol+tamanoSalas)/2);
    Camera camera(jugador,3);
    mazmorra[jugador.getRow()][jugador.getColumn()] = jugador.getBody();
    int Randomtile;
    vector <Monster> monsters;
    for(int i = 0;i < QantMonsters;i++){
        Randomtile = rand()%espaciosDisponibles.size();
        while(mazmorra[espaciosDisponibles[Randomtile].first][espaciosDisponibles[Randomtile].second] != '.'){
            Randomtile = rand()%espaciosDisponibles.size();
        }
        monsters.push_back(Monster("Zombie",20,10));
        mazmorra[espaciosDisponibles[Randomtile].first][espaciosDisponibles[Randomtile].second] = monsters[i].getBody();
    }
    //printDungeon();return;
    system("cls");
     while(true){
        //system("cls");
        camera.updatePosition();
        camera.setPosition(jugador.getRow(),jugador.getColumn());
        camera.printFrame();
        char cmd;
        cmd = getch();
        int dir[4][2] = {
        {0, 1}, {-1, 0}, 
        {1, 0}, {0,-1}
        };
        switch(cmd){
            case 'W': case 'w': case KEY_UP:
                mazmorra[jugador.getRow()][jugador.getColumn()] = '.';
                jugador.move(dir[1][0],dir[1][1]);
                mazmorra[jugador.getRow()][jugador.getColumn()] = 'P';
                break;
            case 'a': case 'A': case KEY_LEFT:
                mazmorra[jugador.getRow()][jugador.getColumn()] = '.';
                jugador.move(dir[3][0],dir[3][1]);
                mazmorra[jugador.getRow()][jugador.getColumn()] = 'P';
                break;
            case 'S': case 's': case KEY_DOWN:
                mazmorra[jugador.getRow()][jugador.getColumn()] = '.';
                jugador.move(dir[2][0],dir[2][1]);
                mazmorra[jugador.getRow()][jugador.getColumn()] = 'P';
                break;
            case 'D': case 'd': case KEY_RIGHT:
                mazmorra[jugador.getRow()][jugador.getColumn()] = '.';
                jugador.move(dir[0][0],dir[0][1]);
                mazmorra[jugador.getRow()][jugador.getColumn()] = 'P';

        }
        Sleep(100);
    }  
}

int main(){
    iniciarJuego();
}