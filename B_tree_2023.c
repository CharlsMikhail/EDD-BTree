//////// IMPLEMENTANDO un B-tree */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct bt_Nodo *b_Tree;

#define MAX_LLAVES (3)

struct bt_Nodo {
    int esHoja;
    int numLLaves;    //////// ¿Cuántas llaves contiene este nodo? */
    int LLaves[MAX_LLAVES];
    struct bt_Nodo *hijos[MAX_LLAVES+1];  //////// Ramificación
};

///////Crear un árbol vacio */
b_Tree btree_Crear(void);

//////// liberar espacio */
void btree_Liberar(b_Tree t);

//////// funcion que devuelve un valor distinto de cero si la llave está presente en el árbol */
int btree_Buscar(b_Tree t, int key);

//////// insertar un nuevo elemento en el árbol */
/////// TAREA
void btree_Insertar(b_Tree t, int key);

//////// imprimir todas las claves del árbol en orden */
//////  TAREA
void btree_Print(b_Tree t);

b_Tree
btree_Crear(void)
{
    b_Tree b;

    b = malloc(sizeof(*b));
    assert(b);

    b->esHoja = 1;
    b->numLLaves = 0;

    return b;
}

void
btree_Liberar(b_Tree b)
{
    int i;

    if(!b->esHoja) {
        for(i = 0; i < b->numLLaves + 1; i++) {
            btree_Liberar(b->hijos[i]);
        }
    }

    free(b);
}

//////// devuelve el índice más pequeño "i" en una matriz ordenada de modo que la llave <= a[i] */
//////// (o n si no existe ese indice) */
static int
btree_BuscarLLave(int n, const int *a, int llave)
{
    int inf;
    int sup;
    int med;


    inf = -1;
    sup = n;

    while(inf + 1 < sup) {
        med = (inf+sup)/2;
        if(a[med] == llave) {
            return med;
        } else if(a[med] < llave) {
            inf = med;
        } else {
            sup = med;
        }
    }

    return sup;
}

int
btree_Buscar(b_Tree b, int llave)
{
    int pos;

    /* es vacio? */
    if(b->numLLaves == 0) {
        return 0;
    }

    /* busque la posición más cercana a la llave  */
    pos = btree_BuscarLLave(b->numLLaves, b->LLaves, llave);

    if(pos < b->numLLaves && b->LLaves[pos] == llave) {
        return 1;
    } else {
        return(!b->esHoja && btree_Buscar(b->hijos[pos], llave));
    }
}
//////// insertar un nuevo elemento en el árbol */
void
btree_Insertar(b_Tree t, int key)
{
    int pos;
    int mid;
    int nuevaLlave;
    struct bt_Nodo *nuevoHijo;
    int i;

    pos = btree_BuscarLLave(t->numLLaves, t->LLaves, key);

    if(pos < t->numLLaves && t->LLaves[pos] == key) {
        return;
    }

    if(t->esHoja) {
        for(i = t->numLLaves; i > pos; i--) {
            t->LLaves[i] = t->LLaves[i-1];
        }
        t->LLaves[pos] = key;
        t->numLLaves++;
    }
    else {
        nuevaLlave = key;
        nuevoHijo = malloc(sizeof(*nuevoHijo));
        assert(nuevoHijo);

        nuevoHijo = t->hijos[pos];

        if(nuevoHijo->numLLaves == MAX_LLAVES) {
            mid = nuevoHijo->numLLaves / 2;

            t->hijos[pos] = malloc(sizeof(*t->hijos[pos]));
            assert(t->hijos[pos]);

            t->hijos[pos+1] = malloc(sizeof(*t->hijos[pos+1]));
            assert(t->hijos[pos+1]);

            t->hijos[pos]->esHoja = 0;
            t->hijos[pos+1]->esHoja = 0;

            t->hijos[pos]->numLLaves = mid;
            t->hijos[pos+1]->numLLaves = nuevoHijo->numLLaves - mid - 1;

            for(i = 0; i < t->hijos[pos]->numLLaves; i++) {
                t->hijos[pos]->LLaves[i] = nuevoHijo->LLaves[i];
            }

            nuevaLlave = nuevoHijo->LLaves[mid];

            for(i = 0; i < t->hijos[pos+1]->numLLaves; i++) {
                t->hijos[pos+1]->LLaves[i] = nuevoHijo->LLaves[mid+1+i];
            }

            for(i = pos+1; i < t->numLLaves; i++) {
                t->hijos[i+1] = t->hijos[i];
            }

            t->hijos[pos]->hijos[t->hijos[pos]->numLLaves+1] = t->hijos[pos+1]->hijos[0];
            t->hijos[pos+1]->hijos[0] = t->hijos[pos+1]->hijos[1];
            t->hijos[pos+1]->hijos[1] = t->hijos[pos+1]->hijos[2];

            t->hijos[pos]->numLLaves++;
            t->hijos[pos+1]->numLLaves--;
        }

        btree_Insertar(t->hijos[pos], nuevaLlave);
    }
}
//////// imprimir todas las claves del árbol en orden */

btree_Print(b_Tree t)
{
    int i;

    if(t->esHoja) {
        for(i = 0; i < t->numLLaves; i++) {
            printf("%d ", t->LLaves[i]);
        }
    } else {
        for(i = 0; i < t->numLLaves; i++) {
            btree_Print(t->hijos[i]);
            printf("%d ", t->LLaves[i]);
        }
        btree_Print(t->hijos[i]);
    }
}

int main() {
////// pruebe su código
    b_Tree t = btree_Crear();

    btree_Insertar(t, 5);
    btree_Insertar(t, 8);
    btree_Insertar(t, 3);
    btree_Insertar(t, 1);
    btree_Insertar(t, 10);

    btree_Print(t);
    printf("\n");

    btree_Liberar(t);
    return 0;
}