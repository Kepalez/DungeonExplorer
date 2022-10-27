#include "Constants.h"

const int MaxFil = 100;
const int MaxCol = 100;

using namespace std;
class Sala{
    private:
    int fil;
    int col;
    int filMap;
    int colMap;
    string puertas;
    public:
    Sala(){}
    ~Sala(){}
    Sala(int fila,int cola){
        fil = fila;
        col = cola;
        puertas = "0000";
    }
    void setPuertas(string nuevasPuertas){puertas = nuevasPuertas;}
    string getPuertas() {return puertas;}
    int getFila(){return fil;}
    int getColumna(){return col;}
    int getFilaMap(){return filMap;}
    int getColumnaMap(){return colMap;}
    bool puertaIzquierda(){return puertas[0] == '0'?  false:true;}
    bool puertaArriba(){return puertas[1] == '0'?  false:true;}
    bool puertaDerecha(){return puertas[2] == '0'?  false:true;}
    bool puertaAbajo(){return puertas[3] == '0'?  false:true;}
    void setFilaMap(int fil){filMap = fil;}
    void setColumnaMap(int col){colMap = col;}
};
int salasTotales = 12;
int salasCreadas = 0;
int tamanoSalas = 6;
char mazmorra[MaxFil][MaxCol]; //Tamano maximo
vector <pair<int,int>> espaciosDisponibles;
bool usados[MaxFil][MaxCol];
//Roberto Bollain
void dibujarSala(Sala act){
    string puertas = act.getPuertas();
    int fil = act.getFila();
    int col = act.getColumna();
    for(int i = fil;i <= fil+tamanoSalas;i++){
        for(int j = col;j <= col+tamanoSalas;j++){
            if(i == fil || j == col || i == fil+tamanoSalas || j == col+tamanoSalas){
                if(i == fil && j == col+(tamanoSalas/2) && puertas[1] == '1'){ //puerta norte
                    mazmorra[i][j] = '.';
                    for(int k = 1;k <= tamanoSalas;k++){
                        mazmorra[i-k][j] = '.';
                        espaciosDisponibles.push_back(pair<int,int>(i-k,j));
                        mazmorra[i-k][j+1] = 178;
                        mazmorra[i-k][j-1] = 178;
                    }
                }
                else if(i == fil+(tamanoSalas/2) && j == col && puertas[0] == '1'){ //puerta oeste
                    mazmorra[i][j] = '.';
                    for(int k = 1;k <= tamanoSalas;k++){
                        mazmorra[i][j-k] = '.';
                        espaciosDisponibles.push_back(pair<int,int>(i,j-k));
                        mazmorra[i-1][j-k] = 178;
                        mazmorra[i+1][j-k] = 178;
                    }
                }
                else if(i == fil+tamanoSalas && j == col+(tamanoSalas/2) && puertas[3] == '1'){ //puerta sur
                    mazmorra[i][j] = '.';
                    for(int k = 1; k <= tamanoSalas;k++){
                        mazmorra[i+k][j] = '.';
                        espaciosDisponibles.push_back(pair<int,int>(i+k,j));
                        mazmorra[i+k][j-1] = 178;
                        mazmorra[i+k][j+1] = 178;
                    }
                }
                else if(i == fil+(tamanoSalas/2) && j == col+tamanoSalas && puertas[2] == '1'){ //puerta este
                    mazmorra[i][j] = '.';
                    for(int k = 1; k <= tamanoSalas;k++){
                        mazmorra[i][j+k] = '.';
                        espaciosDisponibles.push_back(pair<int,int>(i,j+k));
                        mazmorra[i-1][j+k] = 178;
                        mazmorra[i+1][j+k] = 178;
                    }
                }else{
                    mazmorra[i][j] = 178;
                }
            }else{
                mazmorra[i][j] = '.';
                espaciosDisponibles.push_back(pair<int,int>(i,j));
            } 
        }
    }
    //cout<<"Sala registrada\n";
}

void generarDungeon(){
    srand((unsigned) time(NULL));
    for(int i = 0;i < MaxFil;i++){
        for(int j = 0; j < MaxCol;j++){
            mazmorra[i][j] = ' ';
            usados[i][j] = false;
        }
    }
    //cout<<"Empieza proceso\n";
    int fil = MaxFil/2;
    int col = MaxCol/2;
    int filUsados = fil;
    int colUsados= col;
    int conexiones;
    usados[fil][col] = true;
    queue <Sala> salasGeneradas;
    Sala nuevaSala;
    salasGeneradas.push(Sala(fil,col));
    salasGeneradas.front().setFilaMap(fil);
    salasGeneradas.front().setColumnaMap(col);
    int posiblePuerta;
    bool primeraSala = true , creado = false;
    string puertas;
    salasCreadas++;
    while(!salasGeneradas.empty()){
        //cout<<"Sala nueva\n";
        puertas = salasGeneradas.front().getPuertas();
        fil = salasGeneradas.front().getFila();
        col = salasGeneradas.front().getColumna();
        filUsados = salasGeneradas.front().getFilaMap();
        colUsados = salasGeneradas.front().getColumnaMap();
        if(primeraSala){
            conexiones = rand()%4+1;
            primeraSala= false;
        }
        else conexiones = rand()%3+1;
        posiblePuerta= rand()%4;
        while(conexiones > 0){
            creado = true;
            //cout<<"Llega aca\n";
            if(puertas[posiblePuerta] == '0'){
                conexiones--;
                if(salasCreadas < salasTotales){
                    switch(posiblePuerta){
                        case 0:
                            if(!usados[filUsados][colUsados-1]){
                                usados[filUsados][colUsados-1] = true;
                                nuevaSala = Sala(fil,col-tamanoSalas*2);
                                nuevaSala.setPuertas("0010");
                                nuevaSala.setColumnaMap(colUsados-1);
                                nuevaSala.setFilaMap(filUsados);
                                salasGeneradas.push(nuevaSala);
                            }else creado = false;
                            break;
                        case 1:
                            if(!usados[filUsados-1][colUsados]){
                                nuevaSala = Sala(fil-tamanoSalas*2,col);
                                nuevaSala.setPuertas("0001");
                                nuevaSala.setColumnaMap(colUsados);
                                nuevaSala.setFilaMap(filUsados-1);
                                salasGeneradas.push(nuevaSala);
                            }else creado = false;
                            break;
                        case 2:
                            if(!usados[filUsados][colUsados+1]){
                                nuevaSala = Sala(fil,col+tamanoSalas*2);
                                nuevaSala.setPuertas("1000");
                                nuevaSala.setColumnaMap(colUsados+1);
                                nuevaSala.setFilaMap(filUsados);
                                salasGeneradas.push(nuevaSala);
                            }else creado = false;
                            break;
                        case 3:
                            if(!usados[filUsados+1][colUsados]){
                                nuevaSala = Sala(fil+tamanoSalas*2,col);
                                nuevaSala.setPuertas("0100");
                                nuevaSala.setColumnaMap(colUsados);
                                nuevaSala.setFilaMap(filUsados+1);
                                salasGeneradas.push(nuevaSala);
                            }else creado = false;
                    }
                    if(creado){
                        salasCreadas++;
                        puertas[posiblePuerta] = '1';
                    }
                }
            }
            posiblePuerta = (posiblePuerta+1)%4;
        }
        dibujarSala(salasGeneradas.front());
        salasGeneradas.pop();
    }
}

void printDungeon(){
    for(int i = 0;i < MaxFil;i++){
        for(int j = 0;j < MaxCol;j++){
            cout<<mazmorra[i][j];
        }
        cout<<"\n";
    }
}