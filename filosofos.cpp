#include<iostream>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#define F 5 //Número de filosofos
#define probabilidad_pensar 0.5
#define tiempo_pensar 2
#define tiempo_comer 2
using namespace std;
/////////////////////////////////////////////////////////////////////////////////////////////
int turno[F][F] = {}; //Variable Global de turnos.
bool escogiendo[F] = {}; //Variable global de escogiendo. Si esta escogiendo turno.
bool palillo[F] = {}; //Variable global de palillos. Que palillo se esta utilizando.
/////////////////////////////////////////////////////////////////////////////////////////////
struct filosofo{
	int id;//id del filosofo
	int pal_izq;//palillo izquierdo
	int pal_der;//palillo derecho
};
/////////////////////////////////////////////////////////////////////////////////////////////
int Max(int palillo){ 
	int aux = 0;
	for(int i=0;i<F;i++){
		if(turno[palillo][i]>aux) aux=turno[palillo][i];
	}
	return aux;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void *filosofando(void *id){ 
	struct filosofo *respect = (struct filosofo*) id;
	int i = respect->id;
	int pz = respect->pal_izq;
	int pd = respect->pal_der;
	/*Simulamos que el filosofo decide pensar*/
	do{
		while((rand() % 10) < probabilidad_pensar*10){
			printf("El filosofo %i se pone a pensar.\n", i);
			usleep(rand() % ((rand()%tiempo_pensar+1)*100000));
		}
		/*Simulamos que el filosofo decide comer*/
		escogiendo[i] = true; //El filosofo empieza a escoger
		turno[pz][i] = Max(pz)+1; //Asignamos turno al palillo izquierdo dependiendo del filosofo actual
		turno[pd][i] = Max(pd)+1; //Asignamos turno al palillo derecho dependiendo del filosofo actual
		escogiendo[i] = false; //El filosofo termina de escoger
		printf("El filosofo %i le dio hambre y espera el palillo izquierdo %i con turno %i y el palillo derecho %i con turno derecho %i\n", i, pz,turno[pz][i],pd,turno[pd][i]);
		for(int j=0;j<F;j++){
			while(escogiendo[i]);
			//No se continua hasta que el palillo izquierdo este desocupado
			while(palillo[pz] || ((turno[pz][j] != 0) && ((turno[pz][j] < turno[pz][i]) || (( turno[pz][j] == turno[pz][i]) && (j < i)))));
			//No se continua hasta que el palillo derecho este desocupado
			while(palillo[pd] || ((turno[pd][j] != 0) && ((turno[pd][j] < turno[pd][i]) || (( turno[pd][j] == turno[pd][i]) && (j < i)))));
		}
		/*Simulamos que el filosofo pudo tomar los dos palillos a su lado*/
		//El filosofo toma los palillos para preparase a comer
		palillo[pz] = true;
		palillo[pd] = true;
		printf("El filosofo %i tomo los palillos %i y %i \n", i, pz,pd);
		//Sección Crítica
		printf("El filosofo %i empezo a comer\n", i);
		usleep(rand() % ((rand()%tiempo_comer+1)*100000));
		//Salio de la Sección Crítica
		printf("El filosofo %i termina de comer\n", i);
		//El filosofo desocupa los palillos
		palillo[pz] = false;
		palillo[pd] = false;
		printf("El filosofo %i dejo los palillos %i y %i \n", i, pz,pd);
		//El filosofo finaliza su turno con los palillos (Sección Critica)
		turno[pz][i] = 0;
		turno[pd][i] = 0;
	}while(true);
}
/////////////////////////////////////////////////////////////////////////////////////////////
int main(){	
	pthread_t hilo_filosofos[F]; //Creamos threads de los filosofos
	struct filosofo filosofos[F]; //Creamos un arreglo de filosofos tipo filosofo
	srand(time(NULL)); //Semilla del rand
	
	for (int i=0;i<F;i++){
		filosofos[i].id = i; //Le asignamos el id al filosofo i
		filosofos[i].pal_izq = i; //Le asignamos el id al palillo izquierdo
		filosofos[i].pal_der = (i+1)%F; //Le asignamos el id al palillo derecho // Se utiliza el modulo sobre el número de filosofos poque al ultimo filosofo le toca el palillo 0
		//Creamos el thread 
		//Asignamos el puntero al thread, un atributo default, 
		//La función que va a ejecutar el thread y el argumento que pasaremos a la funcion
		pthread_create(&hilo_filosofos[i],NULL,filosofando,&filosofos[i]); 
	}
	pthread_exit(NULL);	//Liberamos memoria de threads
	return 0;
}