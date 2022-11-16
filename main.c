#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>


pthread_mutex_t mxShared;

int repuestos = 15;
int decoracion = 15;
int carpinteria = 15;
int electronicos = 15;
#define Hilos 11


//Se cuenta con 4 hilos de traer que se encargaran de "distribuir" los materiales
//con estos "traer" puede ocurrir una race condition entre el hilo consumidor y el que "distribuye"
void* Traerrep(void* args)
{
    while(1)
    {

        //El lock para que el numero de partes en este caso repuestos no sean modificados y asi solucionar la race condition.
        pthread_mutex_lock(&mxShared);

        repuestos = repuestos + 3;

        //El unlock para que ahora se pueda volver a modificar la variable.
        pthread_mutex_unlock(&mxShared);
        printf("\n3 respuestos han sido distribuidos\n");
        sleep(3);

        if (repuestos > 21)
        {
            return NULL;
        }
    }
}

void* Traerdeco(void* args)
{
    while(1)
    {
        pthread_mutex_lock(&mxShared);
        decoracion = decoracion + 3;
        pthread_mutex_unlock(&mxShared);
        printf("\n3 decoraciones han sido distribuidas\n");
        sleep(3);

        if (decoracion > 21)
        {
            return NULL;
        }
    }
}

void* Traercarp(void* args)
{
    while(1)
    {
        pthread_mutex_lock(&mxShared);
        carpinteria = carpinteria + 3;
        pthread_mutex_unlock(&mxShared);
        printf("\n3 piezas de carpinteria han sido distribuidas\n");
        sleep(3);

        if (carpinteria > 21)
        {
            return NULL;
        }
    }
}

void* Traerelec(void* args)
{
    while(1)
    {
        pthread_mutex_lock(&mxShared);
        electronicos = electronicos + 3;
        pthread_mutex_unlock(&mxShared);
        printf("\n3 piezas de electronicos han sido distribuidas\n");
        sleep(3);

        if (electronicos > 21)
        {
            return NULL;
        }
    }
}


//Con estos "comprar" el hilo se encarga de hacer la accion de descontar la compra elejida simulando la accion del usuario.
//de igual forma aqui se cumple una race condition.
void* comprardeco(void* args)
{
    while(1)
    {
        if (decoracion > 0)
        {
            //Lock para solucionar race condition.
            pthread_mutex_lock(&mxShared);
        
            decoracion = decoracion - 1;

            //unlock para solucionar race condition.
            pthread_mutex_unlock(&mxShared);

            printf("\nSe vendio una pieza de decoracion\n");
            
        }
        
        else
        {
            printf("\nNo hay decoracion disponible en el momento, estamos esperendao los distribuidores.\n");
        }
        sleep(2);

        if (decoracion > 21)
        {
            return NULL;
        }   
    }
}

void* comprarcarp(void* args)
{
    while(1)
    {
        if (carpinteria > 0)
        {
            pthread_mutex_lock(&mxShared);
            carpinteria = carpinteria - 1;
            pthread_mutex_unlock(&mxShared);
            printf("\nSe vendio una pieza de carpienteria\n");
        }      
        else
        {
            printf("\nNo hay carpinteria disponible en el momento, estamos esperando los distribuidores.\n");
        }
        sleep(2);
        if (carpinteria > 21)
        {
            return NULL;
        }   
    }
}

void* comprarelec(void* args)
{
    while(1)
    {
        if (electronicos > 0)
        {
            pthread_mutex_lock(&mxShared);
            electronicos = electronicos - 1;
            pthread_mutex_unlock(&mxShared);
            printf("\nSe vendio una pieza de electronicos\n");
        }
        else
        {
            printf("\nNo hay electronicos disponibles en el momento, estamos esperando los distribuidores.\n");
        }
        sleep(2);
        if (electronicos > 21)
        {
            return NULL;
        }   
    }
}

void* comprarrep(void* args)
{
    while(1)
    {
        if (repuestos > 0)
        {
            pthread_mutex_lock(&mxShared);
            repuestos = repuestos - 1;
            pthread_mutex_unlock(&mxShared);
            printf("\nSe vendio una pieza de repuestos\n");
        }
        else
        {
            printf("\nNo hay repuestos disponibles en el momento, estamos esperando los distribuidores.\n");
        }
        sleep(2);
        if (repuestos > 21)
        {
            return NULL;
        }   
    }
}




//Con estos "bodega" podemos mirar la cantidad de datos que almacena cada variable de piezas.
void* bodegadeco(void* args)
{
    while(1)
    {
        printf("\nTenemos disponibles: %i piezas de decoracion en la bodega\n", decoracion);
        sleep(5);

        if (decoracion > 21)
        {
            return NULL;
        }
    }
    
}

void* bodegacarp(void* args)
{
    while(1)
    {
        printf("\nTenemos disponibles: %i piezas de carpinteria en la bodega\n", carpinteria);
        sleep(5);

        if (carpinteria > 21)
        {
            return NULL;
        }
    }
    
}

void* bodegaelec(void* args)
{
    while(1)
    {
        printf("\nTenemos disponibles: %i piezas de electronicos en la bodega\n", electronicos);
        sleep(5);

        if (electronicos > 21)
        {
            return NULL;
        }
    }
    
}

void* bodegarep(void* args)
{
    while(1)
    {
        printf("\nTenemos disponibles: %i piezas de repuestos en la bodega\n", repuestos);
        sleep(5);
        if (repuestos > 21)
        {
            return NULL;
        }
    }
    
}


int main(int argc, char const *argv[])
{
    pthread_t hilo[Hilos];
    char strlen[2];
    for (int i = 0; i < Hilos; i++)
    {
        hilo[i]=NULL;
    }

    pthread_create(&hilo[0], NULL, &Traerdeco, NULL);
    pthread_create(&hilo[1], NULL, &Traerelec, NULL);
    pthread_create(&hilo[2], NULL, &Traerrep, NULL);
    pthread_create(&hilo[3], NULL, &Traercarp, NULL);
    
    //interfaz
    while(1)
    {
        printf("¿Que tipo de pieza te gustaria comprar?\n");
        printf("1 - Piezas de decoracion.\n");
        printf("2 - Piezas de carpinteria.\n");
        printf("3 - Piezas de electronicos.\n");
        printf("4 - Piezas de repuestos.\n");
        printf("0 - salir\n");
        gets(strlen);

        if (!strcmp(strlen , "1"))
        {
            pthread_create(&hilo[4], NULL, &comprardeco NULL);
        }
        if (!strcmp(strlen , "2"))
        {
            pthread_create(&hilo[5], NULL, &comprarcarp, NULL);
        }
        if (!strcmp(strlen , "3"))
        {
            pthread_create(&hilo[6], NULL, &comprarelec, NULL);
        }
        if (!strcmp(strlen , "4"))
        {
            pthread_create(&hilo[7], NULL, &comprarrep, NULL);
        }
        if (!strcmp(strlen , "0"))
        { 
            printf("\nSalida con exito\n");
            break;
            }
    }
    
    pthread_create(&hilo[8], NULL, &bodegarep, NULL);
    pthread_create(&hilo[9], NULL, &bodegadeco, NULL);
    pthread_create(&hilo[10], NULL, &bodegacarp, NULL);
    pthread_create(&hilo[11], NULL, &bodegaelec, NULL);

    
    for (int i = 0; i < Hilos; i++)
    {
        if(pthread_join(hilo[i],NULL) != 0)
        {   
            perror("Error (hilo %i) ");
        }
    }
    
    pthread_mutex_destroy(&mxShared);
    return 0;
}