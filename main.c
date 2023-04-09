/*
Compilacion: gcc main.c -o Ejecutable -lpthread
Ejecucion: ./Ejecutable
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* Cheff(void*);
void* Mesero(void*);
void* Impresion(void*);

pthread_mutex_t mutex;

struct Cocina{
    int *Comida;
    int *C_Trabajando;
    int *M_Trabajando;
    int *Cheff_ID[3];
    int *Mesero_ID[5];
    pthread_mutex_t *m;
};

int main(int argc, char* argv[]){
    int i, j;
    pthread_mutex_t m1;
    pthread_t Cheffs[3], Meseros[5], Imprimir;
    
    //Inicializamos todas las variables a necesitar
    int Platillos = 0;
    int C_Trabajo = 0;
    int M_Trabajo = 0;
    int ID        = 0;

    struct Cocina C1;
    C1.Comida       = &Platillos;//Inicializamos la cantidad de comida en 0
    C1.C_Trabajando = &C_Trabajo;//Ponemos el estado de trabajo de meseros en 0
    C1.M_Trabajando = &M_Trabajo;
    C1.m            = &m1;       //Variable para la exclusion mutua

    //Creamos los threads necesarios para los Cheffs
    for(i = 0; i < 3; i++){
        ID = i + 1;
        C1.Cheff_ID[i] = &ID;
        if(pthread_create(&Cheffs[i], NULL, Cheff, &C1) != 0){
            printf("Failed to create the Cheff thread!（ミ￣ー￣ミ) \n");
            return 1;
        }  
    }

    ID = 0;

    //Creamos los threada necesarios para los meseros
    for(j = 0; j < 5; j++){
        ID = j + 1;
        C1.Mesero_ID[j] = &ID;
        if(pthread_create(&Meseros[j], NULL, Mesero, &C1) != 0){
            printf("Failed to create the Mesero thread!（ミ￣ー￣ミ) \n");
            return 1;
        }  
    }

    //Creamos el thread de impresion
    if(pthread_create(&Imprimir, NULL, Impresion, &C1) != 0){
        printf("Failed to create the Print thread!（ミ￣ー￣ミ) \n");
        return 1;
    }

    //Entramos a los threads de los Cheffs
    for(i = 0; i < 3; j++){
        if(pthread_join(Cheffs[i], NULL) != 0){
            printf("Failed to join the Cheff thread! (ミ￣ー￣ミ) \n");
            return 2;
        }
    }

    //Entramos a los threads de los meseros
    for(j = 0; j < 5; j++){
        if(pthread_join(Meseros[j], NULL) != 0){
            printf("Failed to join the Mesero thread! (ミ￣ー￣ミ) \n");
            return 2;
        }
    }

    //Entramos al thread de impresion
    if(pthread_join(Imprimir, NULL) != 0){
            printf("Failed to join the Print thread! (ミ￣ー￣ミ) \n");
            return 2;
        }
    
    return 0;
}

void* Cheff(void* args){
    struct Cocina* Cheffsito = (struct Cocina*)args;
    while(1){
        sleep(5);       
        pthread_mutex_lock(&mutex);
        *(Cheffsito->Comida) += 1;
        //printf("La comida actual es de: %d \n", *(Cheffsito->Comida));
        pthread_mutex_unlock(&mutex);
    }
}

void* Mesero(void* args){
    struct Cocina* Trabajador = (struct Cocina*)args;
    while(1){
        if(*(Trabajador->Comida) > 0){
            *(Trabajador->M_Trabajando) = 1;
            sleep(3);
            pthread_mutex_lock(&mutex);
            if(*(Trabajador->Comida) >= 1){
                *(Trabajador->Comida) -= 1;
            }
            pthread_mutex_unlock(&mutex);
            *(Trabajador->M_Trabajando) = 0;
            sleep(1);
        }
        else{
            *(Trabajador->M_Trabajando) = 0;
        }
    }
}

void* Impresion(void* args){
    struct Cocina* Imprime = (struct Cocina*)args;
    system("clear");
    while(1){
        printf("Restaurante el IECSA estresado \n");
        /*
        Nota: La impresion de la comida actual siempre da 0
        porque existen mas meseros que cheffs, y los meseros
        entregan la comida mas rapido de lo que los cheffs la
        preparan.
        Si el sleep de la linea 98 lo cambiamos a sleep(1),
        y el sleep de la linea 112 a sleep(1), entonces se
        veria que la cantidad de comida sube en una unidad 
        cada segundo.
        */
        printf("La comida actual es de: %d \n", *(Imprime->Comida));
        printf("Cheff 1: \n");
        //Aqui estaria el codigo para distingir el chef 1.
        printf("Cheff 2: \n");
        //Aqui estaria el codigo para distingir el chef 2.
        printf("Cheff 3: \n");
        //Aqui estaria el codigo para distingir el chef 3.
        printf("Mesero 1: \n");
        //Aqui estaria el codigo para distingir el mesero 1.
        printf("Mesero 2: \n");
        //Aqui estaria el codigo para distingir el mesero 2.
        printf("Mesero 3: \n");
        //Aqui estaria el codigo para distingir el mesero 3.
        printf("Mesero 4: \n");
        //Aqui estaria el codigo para distingir el mesero 4.
        printf("Mesero 5: \n");
        //Aqui estaria el codigo para distingir el mesero 5.
        sleep(1);
        system("clear");
    }
}
