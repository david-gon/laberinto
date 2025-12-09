#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>

void dfs(int a ,int b,int fila,int columna, int matriz[fila][columna]){
    int movimiento[4][2]={
        {0,1},
        {1,0},
        {0,-1},
        {-1,0}
    };
    //     2,1
    // 3,0 3,1 3,2
    //     4,1

        matriz[a][b]=0;

    for(int i=3; i>0; i--){
    //rand genera cual quier numero al azar y eso lo que hacemos es dividirlo por i+1, ya que da igual el numero que salga, 
    // siempre saldra un numero al azar entre 0 y ese numero, por ejemplo si i=4 entonces sumado 1 sera 5 y todos los numeros que se dividan con 5 sus reciduos saldran 5 
    int j=rand()% (i+1);//guardamos ese nuevo valor en una variable
    int tx=movimiento[i][0], ty=movimiento[i][1];//guardamos la posicion actual para cambiarla por la nueva posicion que generaremos al azar
    //guardamos el valor de la nueva posicion en una posicion nueva
    movimiento[i][0]=movimiento[j][0];
    movimiento[i][1]=movimiento[j][1];
    //y la posicion con la que himos el cambio pasa a estar en el lugar de la posicion original
    movimiento[j][0]=tx;
    movimiento[j][1]=ty;
}
    //probamos todos los movimentos disponibles desde una ubicacion
     for(int k=0; k <4; k++){
        int dx=movimiento[k][0];
        int dy=movimiento[k][1];
        //sumamos el nuevo valor para obtener la nueva direccion y lo multiplicamos por dos para que pueda generace ados casillas
        int nx=a+dx*2;
        int ny=b+dy*2;
        //verificamos si está dentro del laberinto y si el movimiento a dos pasos no está en el borde 
        if (nx>0 && nx<fila && ny>0 && ny<columna && matriz[nx][ny]==1){
            //asignamos el camino en esa posicion
            matriz[a+dx][b+dy]=0;
            //empezamoz la recursion para ir buscando camino hasta que ya no pueda avanzar
            dfs(nx,ny,fila,columna,matriz);
        }
     }
    }
    
void generar_coordenada( int fila, int columna,int matriz[fila][columna]){
 //Generar espacios
    for (int i=0; i<fila;i++){
        for (int j=0; j<columna;j++){
                matriz[i][j]=1;
        }
    }
    dfs(1,1,fila,columna,matriz);
    
}

// generamos estructuras para poder almacenar caminos y almacenarlos dinamicamente
typedef struct {
    int x,y;
}Pos;
typedef struct{
    Pos *datos;
    int usados;//la coleccion de caminos que tenrá
    int capacidad;//la cantidad de datos que tiene
}Camino;

//iniciar el camino
void initcamino(Camino *c){
    c->capacidad=4;//iniciamos la capacidad para guardar al numero en 4bytes
    c->usados=0;//iniciamos la cantidad de caminos recorridos
    c->datos=malloc(c->capacidad*sizeof(Pos));//reserva un valor a la memoria 
    if(!c->datos){//verificamos malloc no collapse en caso de que la memoria ram se llene a causa de sobrecargarla
        perror("malloc");//print error, imprime un el error en caso de que falle el malloc
        exit(EXIT_FAILURE);  // Salir ANTES de que colapse
    }
}
//agregar posicion al camino
void agregarpos(Camino *c, int x, int y){
    if (c->usados==c->capacidad){//en caso de que hayamos llegado al limite
        c->capacidad *=2;//incrementamos la cantidad de bytes
        //realloc espera el puntero de bloque que modificará y el nuevo tamaño
        c->datos=realloc(c->datos, c->capacidad * sizeof(Pos));//asignamos el valor en la memoria
    }
    //guardemos la posicion de x & y
    c->datos[c->usados].x=x;
    c->datos[c-> usados].y=y;
    //aumentamos la cantidad de usados 
    c->usados++;
}
//generamos una funcion para poder copiar los caminos, esto para poder encontrar el camino correcto
void copiar_camino(Camino *destino, Camino *origen){
    destino->capacidad = origen->capacidad;
    destino->usados=origen->usados;
    //asignamos la misma capacidad de memoria
    destino->datos = malloc(sizeof(Pos) * destino->capacidad);
    //en caso de que malloc falle a la hora de guardar un dato por memoria llena retornará null
    if (!destino->datos) { 
        //en ese caso retornamos el valor y cerramos el programa
        perror("malloc copiar_camino"); 
        exit(EXIT_FAILURE); 
    }
    //copiamos con memcpy(destino,origen,num_bytes)
    memcpy(destino->datos, origen->datos, sizeof(Pos) * origen->usados);
}
void eliminar_ultima_pos(Camino *camino) {
    if (camino->usados > 0) {
        camino->usados--;
    }
}

//diseñamos el algoritmo
void generar_laberinto(int fila, int columna,int matriz[fila][columna]){
    // recorrer filas por columna
    for(int i=0; i<fila; i++){
        for(int j=0;j<columna;j++){
            if (matriz[i][j]==1){//en caso de que sea uno colocamos una pared
                printf("██");
            }else if( matriz[i][j]==2){//en caso de que sea 2 colocamos el rastro del camino
                printf("\033[31m██\033[0m");
            }else if (i==fila-2 && j==columna-2){//en caso de que sea la meta colocamos el emoji de "agujero"
                printf("🕳️ ");
            }else{//caso contrario es un espacio
                printf("  ");

            }
        }
        printf("\n");//generamos un salto de linea
    }

}
//generamos una funcion para verficar 
int igual_anterior(int ox, int oy, int nx, int ny){
    if(ox==nx && oy==ny){
        return 0;
    }else{
        return 1;
    }
}

int meta_lograda(int nx,int ny, int fila, int columna){
    if(nx==fila-2 && ny==columna-2){
        return 1;
    }else{
        return 0;
    }
}
//generamos una funcion para poder obtener los movimientos disponibles en un movimiento, sirve para encontrar los nodos hijos
void movimiento_disponible(Camino *explorar,Camino *explorado, int rx, int ry,int fila, int columna,int matriz[fila][columna]){
    int movimiento[4][2]={{0,1},{1,0},{0,-1},{-1,0}};//generamos las 4 direcciones
    Camino temporal;//generamos un camino temporal para guardar los caminos disponibles por cada movimiento
    initcamino(&temporal);
    //guardamos los limites de los que ya se exploró con lo que ya está explorado
    int limite=explorado->usados;
    int limit=explorar->usados;
    for (int i=0; i<limit;i++){//recorremos lo que vamos a explorar(limit) para revisar si es que ya lo tenemos(limite)
        for(int j=0; j<4;j++){//recorremos las 4 direcciones
            int dx=movimiento[j][0], dy=movimiento[j][1]; //guardamos cada direccion en variables
            int x=explorar->datos[i].x, y=explorar->datos[i].y;//guardamos las direcciones actuales
            int nx=x+dx, ny=y+dy;//sumamos las dos direcciones para generar los movimientos
            bool esta=false;//indicamos que ese movimiento que todavia no se guardó en ninguna parte todavia no lo recorrimos
            //verificamos si ese movimiento ya está en nuestra lista de explorados
            for(int k=0; k<explorado->usados;k++){//recorremos toda nuestra lista de explorados para verificar que esteé nx & ny
                if(explorado->datos[k].x==nx && explorado->datos[k].y==ny){
                    esta=true;//tetornamos true y cortamos el ciclo para continuar con otro y que no haga revisar todos
                    break;
                }
            }
            //en caso de que el no haya estado confirmamos que tapoco estee en temporal
            for(int k=0; k<temporal.usados; k++){
                if(temporal.datos[k].x == nx && temporal.datos[k].y==ny){
                    esta=true;//devolveremos tambien true en caso de que sí esté
                    break;//dejamos de buscar cuando ya lo esté
                }
            }
            //confirmamos ademas de que el movimiento no esté fuera del rango y que no sea un camino repetido
            if (nx>=0 && nx<fila && ny>=0 && ny<columna && matriz[nx][ny]!=1 && !esta){
                agregarpos(&temporal,nx,ny);//lo guardamos, y luego continuamos con los otros movimientos
            }
        }
    }
    explorar->usados=0;//borramos todos los datos de explorar, para que se pueda cargar los nuevos
    // for(int i=0; i<temporal.usados; i++){//procedemos a guardar los nuevos valores
    //     //guardamos los nuevos caminos ya que serán ellos los que se utilizarán para el siguiente nivel
    //     agregarpos(explorar,temporal.datos[i].x, temporal.datos[i].y);
    // }
    copiar_camino(explorar,&temporal);
    free(temporal.datos);//depuramos la memoria
}
//generamos un bfs que recorre los caminos correctos
void bfscorrecto(Camino *l, Camino *padres,int x, int y,int fila, int columna,int matriz[fila][columna],bool *meta_encontrada, Camino *resultado_final){ 
    if (*meta_encontrada) return;//verificamos que haya llegado a la meta
    movimiento_disponible(l,padres,x,y,fila,columna,matriz);//buscamos los hijos de los padres 
    for(int i=0; i<l->usados;i++){//recorremos cada hijo por nivel y asu vez abrimos una recursion para poder guardar los datos
        if (*meta_encontrada) break;//revisamos si ya encontró la recursión
        int nx=l->datos[i].x, ny=l->datos[i].y;//guardamos los nuevos valores 
        Camino camino_temporal;//generamos un camino temporal para registrar cada paso hasta llegar a la ruta
        initcamino(&camino_temporal);
        copiar_camino(&camino_temporal,padres);//registramps quienes fueron los padres
        agregarpos(&camino_temporal,nx,ny);//agregamos al hijo
        if(meta_lograda(nx,ny,fila,columna)){ //verficamos si llegamos a la meta 
            copiar_camino(resultado_final, &camino_temporal);//copiamos el camino
            *meta_encontrada=true; //avisamos que la meta ya fue encontrada
            return;     //terminamos recorrido
        } 
        //hacemos una recursión por cada camino recorrido con el fin de dejar el registro
        bfscorrecto(l,&camino_temporal,nx,ny,fila,columna,matriz,meta_encontrada, resultado_final);
        //si llegamos al final y se encontro la meta hacemos el Backtracking
        if(!*meta_encontrada){
            eliminar_ultima_pos(&camino_temporal);
        }
    } 

}

void bfs(Camino *explorar,Camino *explorado, int rx, int ry,int fila, int columna, int matriz[fila][columna],bool *meta_encontrada){
    if (*meta_encontrada) return;// verificamos que se llegó al final
    if (explorar->usados==0) return;//verificamos si es un callejon sin salida
    //obtenemos los hijos disponibles
    movimiento_disponible(explorar,explorado,rx,ry,fila,columna,matriz);
    //revisamos cada hijo devuelto
    for (int i=0; i<explorar->usados;i++){
        //guardamos todos los caminos del mismo nivel
        agregarpos(explorado,explorar->datos[i].x,explorar->datos[i].y);
        int x=explorar->datos[i].x;
        int y=explorar->datos[i].y;
        //verificamos que ya hayamos llegado a la meta para terminar la ejecucion
        if(meta_lograda(x,y,fila,columna)){
            *meta_encontrada=true;
                return;
        }
    }
    bfs(explorar,explorado,rx,ry,fila,columna,matriz,meta_encontrada);//volvemos a enviar los datos

}

void encontrar_camino(Camino *camino,Camino *correcto,int ux, int uy,int rx,int ry,int fila,int columna,int matriz[fila][columna],bool *meta_encontrada){
    if (*meta_encontrada) return;  //revisar si la meta ya fue encontrada
    //movimientos anteriores
    int ox=rx;
    int oy=ry;
    int movimiento[4][2]={{0,-1},{-1,0},{0,1},{1,0}};//movimientos posibles
    for (int i=0; i<4; i++){
        int dx=movimiento[i][0], dy=movimiento[i][1];//direciones
        int nx=rx+dx,ny=ry+dy;//nuevas direcciones
        //verificar movimientos disponibles
        int iguales=igual_anterior(ux,uy,nx,ny);
        if (nx>=0 && nx<fila && ny>=0 && ny<columna && matriz[nx][ny]!=1 && iguales==1){
            agregarpos(camino,nx,ny);//agregamos el camino recorrido
            agregarpos(correcto,nx,ny);//agregamos el camino correcto
            if (meta_lograda(nx,ny,fila,columna)){
                // generar_laberinto(nx,ny,fila,columna,matriz);
                *meta_encontrada=true;
                return;
            }
            encontrar_camino(camino,correcto,ox,oy,nx,ny,fila,columna,matriz,meta_encontrada);
        }    
    }
    if (!(*meta_encontrada)) {
        eliminar_ultima_pos(correcto);
    }
    }

void clonar_matriz(int fila, int columna, int matriz[fila][columna], int matriz_copia[fila][columna]){
    for (int i=0; i<fila; i++){
        for(int j=0; j<columna; j++){
            matriz_copia[i][j]=matriz[i][j];
        }
    }
}
int main(){
        //definicion de posiciones y limites
        int fila,columna;
        int rx=1,ry=1;
        int ux=1;
        int uy=1;
        int busqueda;
        //definir dimension del laberinto
        srand(time(NULL));
        printf("ingrese el ancho");
        scanf("%d",&fila);
        int metax=fila-2;
        printf("ingrese el largo");
        scanf("%d",&columna);
        int metay=columna-2;
        int matriz[fila][columna];
        int raton[rx][ry];
        matriz[rx][ry]=2;
        
        //generar coordenada
        generar_coordenada(fila,columna, matriz);
        // Copiar toda la matriz
        //generar estructuras dinamicas para guaradar los pasos del dfs
        Camino camino;
        initcamino(&camino);
        agregarpos(&camino, 1,1);
        Camino correcto;
        initcamino(&correcto);
        agregarpos(&correcto,1,1);
        //generar estructuras dinamicas para guaradar los hijos de un mismo nivel
        Camino explorar;
        initcamino(&explorar);
        agregarpos(&explorar,1,1);
        //generar estructuras dinamicas para guaradar todos los pasos que se recorrió
        Camino explorado;
        initcamino(&explorado);
        agregarpos(&explorado,1,1);
        //generar estructuras dinamicas para guaradar los pasos de camino
        Camino bfs_corect_expl;
        initcamino(&bfs_corect_expl);
        agregarpos(&bfs_corect_expl,1,1);
        Camino bfs_correcto;
        initcamino(&bfs_correcto);
        agregarpos(&bfs_correcto,1,1);
        generar_laberinto(fila,columna,matriz);

        int matriz_copia[fila][columna];
        clonar_matriz(fila, columna, matriz, matriz_copia);

        Camino resultado;
        initcamino(&resultado);     
        bool meta_encontrada=false;
        printf("para resolver el laberinto eliga un metodo de resolucion\n");
        printf("1) DFS (Depth-Firts Search)\n");
        printf("2) BFS (Breadth-First Search)\n");
        scanf("%d",&busqueda);
        if(busqueda==1){
            clock_t inicio=clock();
            encontrar_camino(&camino,&correcto,ux,uy,1,1,fila,columna,matriz,&meta_encontrada);
            clock_t fin=clock();
            for (int i=0; i<camino.usados;i++){
                int x=camino.datos[i].x;
                int y=camino.datos[i].y;
                matriz[x][y]=2;
                Sleep(500);
                system("cls");
                generar_laberinto(fila,columna,matriz);
            }
            double tiempo_ejecucion = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
            printf("tardo %f estos segundos en recorrer\n", tiempo_ejecucion);
            char validardfs;
            printf("desea ver el resultado y/n: ");
            scanf(" %c",&validardfs);
            if (validardfs=='y'){
                for (int i=0;i<correcto.usados;i++){
                    int x=correcto.datos[i].x;
                    int y=correcto.datos[i].y;
                    matriz_copia[x][y]=2;
                    Sleep(500);
                    system("cls");
                    generar_laberinto(fila,columna,matriz_copia);
                }

            }
        }else if(busqueda==2){
            // Reiniciar estructuras de camino
            clock_t inicio=clock();
            bfs(&explorar,&explorado,1,1,fila,columna,matriz,&meta_encontrada);
            clock_t fin=clock();

            for (int i=0;i<explorado.usados;i++){
                int x=explorado.datos[i].x;
                int y=explorado.datos[i].y;
                matriz[x][y]=2;
                Sleep(500);
                system("cls");
                generar_laberinto(fila,columna,matriz);
                
            }
            explorado.usados=0;
            char validar;
            double tiempo_ejecucion = ((double)(fin - inicio)) / CLOCKS_PER_SEC;
            printf("tardo %f estos segundos en recorrer\n", tiempo_ejecucion);
            printf("desea ver el resultado y/n");
            scanf(" %c",&validar);
            meta_encontrada = false;
            if (validar=='y'){
                bfscorrecto(&bfs_correcto,&bfs_corect_expl,1,1,fila,columna,matriz_copia,&meta_encontrada,&resultado);
                for (int i=0;i<resultado.usados;i++){
                    // printf("entramos");
                    int x=resultado.datos[i].x;
                    int y=resultado.datos[i].y;
                    matriz_copia[x][y]=2;
                    printf("(%d,%d)\n",x,y);
                    Sleep(500);
                    system("cls");
                    generar_laberinto(fila,columna,matriz_copia);
                }

            }

        }
            // Liberar memoria
    free(camino.datos);
    free(explorar.datos);
    free(explorado.datos);
    free(bfs_corect_expl.datos);
    free(bfs_correcto.datos);
    free(resultado.datos);
        return 0;
}
