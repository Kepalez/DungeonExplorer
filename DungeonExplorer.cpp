#include "Constants.h"
#include "Camera.h"

const int QantMonsters = 7;
const int QantCoins = 15;
vector <Monster> monsters;
int playerRow, playerCol;
Camera camera;
int points = 0;
int playerHP = 0;
bool running = true;
bool win = false;
int dir[4][2] = {
        {0, 1}, {-1, 0}, 
        {1, 0}, {0,-1}
        };
void gotoxy(int line, int column){
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
}
void *playerMove(void *arg){
    Hero player = *((Hero*)arg);
    camera = Camera(player,4);
    camera.setPosition(player.getRow(),player.getColumn());
    char cmd = 0;
    fflush(stdin);
    int tryCol,tryRow;
    playerRow = player.getRow();
    playerCol = player.getColumn();
    playerHP = player.getRemainingHP();
    while(running){
        cmd = getch();
        switch(cmd){
            case 'W': case 'w': case KEY_UP:
                tryRow = player.getRow()+dir[1][0];
                tryCol = player.getColumn()+dir[1][1];
                break;
            case 'a': case 'A': case KEY_LEFT:
                tryRow = player.getRow()+dir[3][0];
                tryCol = player.getColumn()+dir[3][1];
                break;
            case 'S': case 's': case KEY_DOWN:
                tryRow = player.getRow()+dir[2][0];
                tryCol = player.getColumn()+dir[2][1];
                break;
            case 'D': case 'd': case KEY_RIGHT:
                tryRow = player.getRow()+dir[0][0];
                tryCol = player.getColumn()+dir[0][1];
                break;
            case '0':
                running = false;
        }
        switch(mazmorra[tryRow][tryCol]){
            case '.':
                mazmorra[player.getRow()][player.getColumn()] = '.';
                player.setPosition(tryRow,tryCol);
                mazmorra[player.getRow()][player.getColumn()] = player.getBody();
                break;
            case 'E':
                system("cls");
                running = false;
                win = true;
                break;
            case 'M':
                for(int i = 0;i < QantMonsters;i++){
                    if(monsters[i].getColumn() == tryCol && monsters[i].getRow()==tryRow){
                        player.recieveDmg(monsters[i].getDMG());
                        monsters[i].recieveDmg(monsters[i].getDMG());
                        if(monsters[i].isDead()){
                            mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = '.';
                            points+=rand()%501+500;
                        }
                        if(player.isDead()){
                            system("cls");
                            running = false;
                        }
                    }
                }
                break;
            case 'C':
                mazmorra[player.getRow()][player.getColumn()] = '.';
                player.setPosition(tryRow,tryCol);
                mazmorra[player.getRow()][player.getColumn()] = player.getBody();
                points+= rand()%201+500;
                break;
        }
        playerHP = player.getRemainingHP();
        playerRow = player.getRow();
        playerCol = player.getColumn();
        camera.setPosition(player.getRow(),player.getColumn());
    }
    pthread_exit(0);
}
void *enemyMove(void *arg){
    int x;
    int rowDif, colDif;
    while(running){
        for(int i = 0;i < QantMonsters;i++){
            if(!monsters[i].isDead()){
                if(monsters[i].getStatus() == "Idle"){
                x = rand()%4;
                if(mazmorra[monsters[i].getRow()+dir[x][0]][monsters[i].getColumn()+dir[x][1]] == '.'){
                    mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = '.';
                    monsters[i].move(dir[x][0],dir[x][1]);
                    mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = 'M';
                }
                rowDif = abs(playerRow - monsters[i].getRow());
                colDif = abs(playerCol - monsters[i].getColumn());
                if(rowDif + colDif <= 2){
                    monsters[i].setLastSeen(playerRow,playerCol);
                    monsters[i].setStatus("Chasing");
                }
                }else if(monsters[i].getStatus()=="Chasing"){
                    rowDif = abs(playerRow - monsters[i].getRow());
                    colDif = abs(playerCol - monsters[i].getColumn());
                    if(rowDif + colDif <= 4){
                        monsters[i].setLastSeen(playerRow,playerCol);
                    }
                    rowDif = abs(monsters[i].getLastSeen().first - monsters[i].getRow());
                    colDif = abs(monsters[i].getLastSeen().second - monsters[i].getColumn());
                    if(colDif == 0 || (rowDif < colDif && rowDif != 0)){
                        if(monsters[i].getLastSeen().first < monsters[i].getRow()){ //jugador esta mas arriba
                            if(mazmorra[monsters[i].getRow() - 1][monsters[i].getColumn()] == '.'){
                                mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = '.';
                                monsters[i].move(-1,0);
                                mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = 'M';
                            }
                        }else{ //Jugador esta mas abajo
                            if(mazmorra[monsters[i].getRow() + 1][monsters[i].getColumn()] == '.') {
                                mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = '.';
                                monsters[i].move(1,0);
                                mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = 'M';
                            }
                        }
                    }else if(rowDif == 0 || (colDif < rowDif && colDif != 0)){ 
                        if(monsters[i].getLastSeen().second < monsters[i].getColumn()){ //Jugador esta a la izquierda
                            if(mazmorra[monsters[i].getRow()][monsters[i].getColumn()-1] == '.'){
                                mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = '.';
                                monsters[i].move(0,-1);
                                mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = 'M';
                            }
                        }else{ //Jugador esta a la derecha
                            if(mazmorra[monsters[i].getRow()][monsters[i].getColumn()+1] == '.'){
                                mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = '.';
                                monsters[i].move(0,1);
                                mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = 'M';
                            }
                        }
                    }else if(colDif == rowDif){
                        if(rand()%2 == 0){ //intentar camino vertical
                            if(monsters[i].getLastSeen().first < monsters[i].getRow()){ //jugador esta mas arriba
                            if(mazmorra[monsters[i].getRow() - 1][monsters[i].getColumn()] == '.'){
                                mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = '.';
                                monsters[i].move(-1,0);
                                mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = 'M';
                            }
                            }else{ //Jugador esta mas abajo
                                if(mazmorra[monsters[i].getRow() + 1][monsters[i].getColumn()] == '.') {
                                    mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = '.';
                                    monsters[i].move(1,0);
                                    mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = 'M';
                                }
                            }
                        }else{ // intentar camino horizontal
                            if(monsters[i].getLastSeen().second < monsters[i].getColumn()){ //Jugador esta a la izquierda
                            if(mazmorra[monsters[i].getRow()][monsters[i].getColumn()-1] == '.'){
                                mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = '.';
                                monsters[i].move(0,-1);
                                mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = 'M';
                            }
                            }else{ //Jugador esta a la derecha
                                if(mazmorra[monsters[i].getRow()][monsters[i].getColumn()+1] == '.'){
                                    mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = '.';
                                    monsters[i].move(0,1);
                                    mazmorra[monsters[i].getRow()][monsters[i].getColumn()] = 'M';
                                }
                            }
                        }
                    }
                    if(monsters[i].getRow() == monsters[i].getLastSeen().first && monsters[i].getColumn() == monsters[i].getLastSeen().second){
                        monsters[i].setStatus("Idle");
                    }
                }
            }   
        }
        Sleep(300);
    }
    pthread_exit(0);
}

void startGame(){
    generarDungeon();
    cout<<"What's your name?";
    string name;
    cin>>name;
    system("cls");
    gotoxy(0,0);
    cout<<"Welcome to the dungeon, "<<name;
    Sleep(2000);
    Hero player(name,50,15);
    player.setPosition((MaxFil+tamanoSalas)/2,(MaxCol+tamanoSalas)/2);
    mazmorra[player.getRow()][player.getColumn()] = player.getBody();
    int Randomtile;
    
    for(int i = 0;i < QantMonsters;i++){
        Randomtile = rand()%espaciosDisponibles.size();
        while(mazmorra[espaciosDisponibles[Randomtile].first][espaciosDisponibles[Randomtile].second] != '.'){
            Randomtile = rand()%espaciosDisponibles.size();
        }
        monsters.push_back(Monster("Zombie",20,5));
        mazmorra[espaciosDisponibles[Randomtile].first][espaciosDisponibles[Randomtile].second] = monsters[i].getBody();
        monsters[i].setPosition(espaciosDisponibles[Randomtile].first,espaciosDisponibles[Randomtile].second);
    }
    for(int i = 0;i <QantCoins;i++){
        Randomtile = rand()%espaciosDisponibles.size();
        while(mazmorra[espaciosDisponibles[Randomtile].first][espaciosDisponibles[Randomtile].second] != '.'){
            Randomtile = rand()%espaciosDisponibles.size();
        }
        mazmorra[espaciosDisponibles[Randomtile].first][espaciosDisponibles[Randomtile].second] = 'C';
    }
    pthread_t EnemyThread,PlayerThread;
    //printDungeon();return;
    system("cls");
    pthread_create(&EnemyThread,NULL,&enemyMove,NULL);
    pthread_create(&PlayerThread,NULL,&playerMove,(void*)&player);
    gotoxy(3,30);
    cout<<player.getName();
    //cout<<"HP: "<<camera.getTarget().getRemainingHP();
    while(running){
        camera.printFrame();
        gotoxy(4,30);
        cout<<"HP:             ";
        gotoxy(4,30);
        cout<<"HP: "<<playerHP;
        gotoxy(5,30);
        cout<<"Points: "<<points;
        Sleep(50);
    }
    //GAME OVER
    pthread_join(EnemyThread,0);
    pthread_join(PlayerThread,0);
    ifstream pantalla;
    string texto;
    if(win){
        gotoxy(0,0);
        pantalla.open("YOUWIN.txt",ios::in);
        while(!pantalla.eof()){
            getline(pantalla,texto);
            cout<<texto<<"\n";
        }
        pantalla.seekg(0, ios::beg);
        pantalla.close();
        Sleep(1000);
        system("pause");
    }else{
        pantalla.open("GAMEOVER.txt");
        gotoxy(0,0);
        while(!pantalla.eof()){
            getline(pantalla,texto);
            cout<<texto<<"\n";
        }
        pantalla.seekg(0,ios::beg);
        Sleep(1000);
        system("pause");
    }
}

void mainMenu(){
    int option = 1;
    string texto;
    char cmd;
    ifstream pantalla;
    pantalla.open("MenuSelectGame.txt",ios::in);
    while(!pantalla.eof()){
            getline(pantalla,texto);
            cout<<texto<<"\n";
    }
    pantalla.seekg(0, ios::beg);
    pantalla.close();
    while(true){        //Loop Starts
        while(!pantalla.eof()){
            getline(pantalla,texto);
            cout<<texto<<"\n";
        }
        cmd = getch();
        switch(cmd){    //Navigate between options or select
        case 'W': case 'w': case KEY_UP:
            option--;
            if(option == 0) option = 3;
            break;
        case 'S': case 's': case KEY_DOWN:
            option++;
            if(option == 4) option = 1;
            break;
        case '\r':
            switch(option){
                case 1: // Start Game
                system("cls");
                pantalla.seekg(0,ios::beg);
                pantalla.close();
                pantalla.open("GameTitle1.txt",ios::in);
                while(!pantalla.eof()){
                    getline(pantalla,texto);
                    cout<<texto<<"\n";
                }
                pantalla.seekg(0, ios::beg);
                pantalla.close();
                Sleep(500);
                system("cls");
                pantalla.open("GameTitle2.txt",ios::in);
                while(!pantalla.eof()){
                    getline(pantalla,texto);
                    cout<<texto<<"\n";
                }
                pantalla.seekg(0, ios::beg);
                pantalla.close();
                Sleep(500);                
                system("cls");
                pantalla.open("GameTitle3.txt",ios::in);
                while(!pantalla.eof()){
                    getline(pantalla,texto);
                    cout<<texto<<"\n";
                }
                pantalla.seekg(0, ios::beg);
                Sleep(500);
                pantalla.close();
                system("cls");
                Sleep(500);
                
                startGame();
                break;
                case 2:
                //Open ScoreBoard
                break;
                case 3:
                break;
                //Exit Game
            }
            system("cls");

            return;
        }
        switch(option){     //Print option
            case 1: //Open game option
            pantalla.open("MenuSelectGame.txt",ios::in);
            break;
            case 2:
            pantalla.open("MenuSelectHighScore.txt",ios::in);
            //Scoreboard option
            break;
            case 3:
            pantalla.open("MenuSelectExit.txt",ios::in);
            //Exit game option
        }
        system("cls");
        while(!pantalla.eof()){
            getline(pantalla,texto);
            cout<<texto<<"\n";
        }
        pantalla.seekg(0, ios::beg);
        pantalla.close();
    }
}

int main(){
    system("chcp 65001");
    system("cls");
    mainMenu();
}