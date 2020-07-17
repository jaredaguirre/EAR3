/**//* 39.336.695-VACA_AGUIRRE,JaredMartin-(05-1965) *//**/


/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
/**//**//* CUALQUIER INCLUDE DE BIBLIOTECA QUE NECESITE, HÁGALO ACÁ   *//**//**/

#include <string.h>
#include <stdlib.h>
#include <math.h>

/**//**//* CUALQUIER INCLUDE DE BIBLIOTECA QUE NECESITE, HÁGALO ACÁ   *//**//**/
/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
#include "funciones.h"
/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
/**//**//* ACÁ DEBE DESARROLLAR LAS FUNCIONES Y PRIMITIVAS PEDIDAS    *//**//**/
/**//**//* ADEMÁS DE CUALQUIER OTRA FUNCIÓN QUE SE REQUIERA           *//**//**/


/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/

/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
/**//* FUNCIONES Y PRIMITIVAS A DESARROLLAR                               *//**/
/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/
/**//* para la información                                                *//**/

void mostrarFinal_MIO(const void *d, FILE *fp)
{
    //Volcamos el dato void a un tipo tFinal
    const tFinal* a = (const tFinal*)d;

    /*
    Si el dato volcado es NULL, entonces imprimimos el "header" o "cabecera"
    Si no, imprimimos la linea completa.
    */
    if(!a)
        fprintf(fp, " D. N. I.  Apellido(s), Nombre(s)         CodM Cal\n");
    else fprintf(fp, "%10s %-31s%4d %2d\n", a->dni, a->apYNom, a->codMat, a->calif);

}

int  compararFinal_MIO(const void *d1, const void *d2)
{
    return strcmp(((const tFinal*)d1)->dni, ((const tFinal*)d2)->dni);
}


int  acumularCalif_MIO(void **dest, unsigned *tamDest,
                       const void *orig, unsigned tamOrig)
{
    tFinal **d = (tFinal**)dest;
    tFinal * o = (tFinal*)orig;

    (*d)->calif += o->calif;
    return 1;
}

void mostrarProm_MIO(const void *d2, int cantCalif, FILE *fp)
{
    fprintf(fp, "   --- Rindio  %d final(es) con un promedio de: %2.0f\n\n",
            cantCalif,
            round(((tFinal*)d2)->calif / (float)cantCalif));
}


/**//* para el TDA LISTA                                                  *//**/

int  mostrarDeIzqADer_MIO(const tLista *p,
                          void(*mostrar)(const void *, FILE *), FILE *fp)
{
    /**
    ACLARACIÓN: Esta función no cumple el cometido que el nombre indica.
    De acuerdo a las aclaraciones solicitadas, muestra de DERECHA a IZQUIERDA
    */
    int cant = 0;           //Cantidad de elementos mostrados
    tNodo * fin = *p;       //Puntero que va al final de la lista

    //Ponemos fin al final
    while(fin->sig)
        fin = fin->sig;

    //Mostramos la Cabecera de la información
    mostrar(NULL, fp);

    //Mientras "fin" no sea NULL,
    while(fin)
    {
        mostrar(fin->info, fp); //Mostramos la información
        fin = fin->ant;         //"fin" avanza al anterior
        cant++;                 //Acumulamos la cantidad
    }
    return cant;                //Retornamos la cantidad acumulada
}


void ordenarLista_MIO(tLista *p, int (*comparar)(const void *, const void *))
{
    /**
    CÓMO FUNCIONA?
    --------------
    La lista se recorrerá de fin a principio. Un puntero "ACT" que comienza al final
    irá apuntando a cada nodo, y se evaluará la cantidad de veces que cada DNI de cada
    nodo aparece en la lista.

    A medida que se recorre, se irá comparando las repeticiones encontradas
    con las ocurrencias buscadas. Si las repeticiones son iguales a la cantidad de
    ocurrencias buscadas, entonces ACT habrá encontrado el DNI que debemos usar como referencia.

    Al encontrar dicho DNI, comenzamos a "barrer" la lista de fin a principio, desvinculando
    cada nodo que tenga un DNI igual al que encontramos con ACT. Una vez desvinculados y llevados
    a una "lista paralela", el ciclo se inicia de nuevo, hasta que ACT termine de "barrer" toda
    la lista. Una vez que ACT llegó al final, entonces el ciclo se reinicia, esta vez aumentando
    en 1 las ocurrencias buscadas. El proceso termina cuando INI queda apuntando a NULL.

    ACLARACIÓN: La "lista paralela" NO es literalmente una nueva lista. Es solamente una forma de
    llamar al segmento donde iremos poniendo los desvinculados con el objeto de organizarlos.
    En ningún momento se crea un nuevo nodo.

    DEFINICIÓN DE PUNTEROS Y VARIABLES:
    -----------------------------------
    ocurrencias = Las ocurrencias buscadas actualmente. A medida que el ciclo se repita
                  irá aumentando de a 1.
    cantEncontrados = Aqui iremos guardando la cantidad de nodos encontrados de cada DNI

    dniRef = DNI Referencia. Una vez que encontremos el DNI que debemos desvincular de la
             la lista, aquí guardaremos dicha cadena, para luego ser comparada.

    paralelo = El puntero que apunta a la "lista paralela", donde guardamos los desvinculados.

    ACT = Puntero al nodo Actual, que se usa para evaluar la cantidad de repeticiones de cada
          DNI al que este puntero apunta.

    INI = Puntero a INICIO. A fin de "barrer" la lista completa, necesitamos siempre la dirección
          del comienzo desde donde vamos a recorrer. En este caso INI siempre apunta al final.

    R y RD = Punteros que se usan para recorrer la lista, ya sea para contar las repeticiones de ACT (R)
             o bien para recorrerla en busca de los que debemos devincular (RD).
    */
    int ocurrencias = 1,
        cantEncontrados;

    tNodo *paralelo = NULL,
          *act,
          *ini = *p,
          *r;

    char dniRef[11];

    //Ponemos INI y ACT al final de la lista
    while(ini->sig)
        ini = ini->sig;
    act = ini;

    //Mientras INI sea parte de la lista
    while(ini)
    {
        //Si ACT es NULL,
        //COMIENZA OTRA BÚSQUEDA
        if(!act)
        {
            ocurrencias++;    //Aumentamos las ocurrencias buscadas
            act = ini;        //ACT se posiciona al final
        }

        r = ini;              //R se posiciona al final
        cantEncontrados = 0;  //La cantidad de encontrados se resetea a 0

        //Mientras R no llegue a NULL, y no haya más encontrados de los que buscamos...
        while(r && cantEncontrados <= ocurrencias)
        {
            //Si ACT y R tienen el mismo DNI,
            if(!comparar(act->info, r->info))
                cantEncontrados++;  //Aumenta la cantidad de encontrados

            r = r->ant;             //Luego R Avanza al anterior
        }

        /*Si los elementos encontados coinciden con las ocurrencias que buscamos...
          Comenzamos la desvinculación...*/
        if(cantEncontrados == ocurrencias)
        {
            //DESVINCULACIÓN DE NODO(S)
            strcpy(dniRef, ((tFinal*)act->info)->dni);  //Nos guardamos el DNI usamos para comparar
            tNodo *rd = ini,                            //Invocamos el puntero que recorre la lista
                  *aux = NULL;                          //Y el que nos ayuda a no perder posiciones

            //Mientras RD esté recorriendo la lista
            while(rd)
            {
                //Si el DNI de RD es igual al buscado, comienza el cableado
                if(!strcmp(((tFinal*)rd->info)->dni, dniRef))
                {
                    aux = rd->ant;          //AUX se prepara para recuperar RD cuando sea desvinculado

                    if(ini == rd)           //Si RD está donde INI,
                        ini = ini->ant;     //Movemos INI para no perderlo

                    //Si RD tiene anterior, y no cablea a NULL
                    if(rd->ant && (rd->ant->sig = rd->sig) != NULL)
                        rd->sig->ant = rd->ant;             //El cableado sigue del otro lado

                    else if(rd->sig)                        //Si tiene siguiente,
                        rd->sig->ant = rd->ant;             //Cablea desde el otro lado

                    //Agregamos lo quitado a la "lista paralela"
                    if(!paralelo)
                    {
                        //Si se trata de la primer desvinculación
                        paralelo = rd;
                        paralelo->sig = NULL;
                    }
                    else
                    {
                        //Caso sea una segunda desvinculación o posterior
                        paralelo->ant = rd;
                        paralelo->ant->sig = paralelo;
                        paralelo = paralelo->ant;
                    }
                    //Por ser lista doble, siempre hay un NULL en los extremos.
                    paralelo->ant = NULL;

                    //Si ACT y RD fueron desvinculados (y RD siempre lo será), los recuperamos con AUX
                    if(act == rd)
                        act = aux;
                    rd = aux;
                }
                //Si no, RD avanza al anterior
                else rd = rd->ant;
            }
        }
        //Si no, ACT avanza al anterior
        else act = act->ant;
    }
}


int  vaciarMostrarDetalleYProm_MIO(tLista *p, FILE *fp,
                                   int (*compararFinal_MIO)(const void *, const void *),
                                   int (*acumularCalif_MIO)(void **, unsigned *,
                                                   const void *, unsigned),
                                   void (*mostrarFinal_MIO)(const void *, FILE *),
                                   void (*mostrarProm_MIO)(const void *, int, FILE *))
{
    /*
    DEFINICIÓN DE PUNTEROS Y VARIABLES:

    cantElim = Cantidad de nodos eliminados
    cantCalif = Cantidad de Calificaciones

    ACT = Es el puntero que se posiciona al comienzo de cada segmento. Una vez que
          ACT se posiciona, a partir del mismo se desprende el puntero AUX.
    AUX = Puntero que se desprende de ACT con el fin de recorrer los anteriores
          nodos para verificar si tienen el mismo DNI.

    AUX2= Funciona como puntero de existencia temporal para eliminar los nodos
    */
    int cantElim = 0,
        cantCalif;

    tNodo *act = *p;

    //Si ACT no es NULL, se da comienzo. De lo contrario, retornamos CERO.
    if(act)
    {
        //Llevamos ACT hasta el final de la lista
        while(act->sig)
            act = act->sig;

        //Mientras ACT no sea NULL
        while(act)
        {
            cantCalif = 0;                      //Comenzamos a contar calificaciones
            tNodo *aux = act->ant;              //AUX se invoca como el anterior de ACT
            mostrarFinal_MIO(NULL, fp);         //Mostramos la cabecera del segmento a eliminar
            mostrarFinal_MIO(act->info, fp);    //Mostramos el primer Registro
            cantCalif++;                        //Aumenta el contador de calificaciones

            //PROCESO DE ACUMULACIÓN:
            //Mientras AUX tenga el mismo dni que ACT
            while(aux && compararFinal_MIO(act->info, aux->info) == 0)
            {
                mostrarFinal_MIO(aux->info, fp);                                            //Mostramos AUX
                cantCalif++;                                                                //Aumenta el contador de calificaciones
                acumularCalif_MIO(&act->info, &(act->tamInfo), aux->info, aux->tamInfo);    //Acumulamos la nota en ACT
                aux = aux->ant;                                                             //Avanza AUX al anterior
            }
            mostrarProm_MIO(act->info, cantCalif, fp);  //Mostramos el Promedio
            if(aux)
                aux->sig = NULL;                        //Desvinculamos todo lo posterior a AUX

            //PROCESO DE ELIMINACIÓN:
            //Mientras ACT sea diferente de AUX
            while(act != aux)
            {
                tNodo *aux2 = act;  //Invocamos un AUX2 en la posicion de ACT
                act = act->ant;     //ACT Avanza al anterior
                free(aux2->info);   //liberamos la info y el nodo
                free(aux2);
                cantElim++;         //Aumenta cantidad de eliminados
            }
        }
        *p = NULL;      //a P se le asigna valor NULL, ya que la lista está vacía
    }
    return cantElim;

}


/**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/

