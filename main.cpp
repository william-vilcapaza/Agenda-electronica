#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include <string.h>
#include <conio.h>
#define CLEAR_SCREEN "cls"


typedef struct ADTContacto {
    char* nombre;
    char* dni;
    char* sexo;
    char* fecha_nac;
    char* telefono_movil;
    char* correo_electronico;
    char* direccion;
} Contacto;

typedef struct ParbolBinario {
    struct ParbolBinario *izquierda;
    struct ParbolBinario *derecha;
    void *dato;
} *arbolBinario;

arbolBinario insertarArbol    (arbolBinario root, int (*compare)(const void*, const void*), void *dato);
void*      buscarArbol    (arbolBinario root, int (*compare)(const void*, const void*), const void *dato);
arbolBinario borrarArbol    (arbolBinario root, int (*compare)(const void*, const void*), const void *dato);
void       mostralArbol  (arbolBinario root, void *params, void (*func)(void*, void*));
void       arbol_inorder   (arbolBinario root, void *params, void (*func)(void*, void*));
void  pause_screen (const char *mensaje);
char* read_string_line (const char *mensaje);
char* read_str (const char *mensaje, char *string_var);
int   read_integer (const char *mensaje);
int   read_field (FILE *file_stream, char *field);
int   compara_contacto (const Contacto *contacto1, const Contacto *contacto2);
void  imprime_contacto (Contacto *dato, int *contador);
void  imprimir_en_archivo (Contacto *dato, FILE *file_stream);

const char *path = "contactos.tsv";

int main (void)
{
    arbolBinario root = NULL;
    Contacto *dato = NULL, contacto;
    int contador=0, opcion_principal, subopcion;
    char field[255];
    FILE *file_stream = fopen (path, "r");
    if (file_stream!=NULL)
    {
        while (read_field (file_stream, field))
        {
            dato = (Contacto*) malloc (sizeof (Contacto));
            dato->nombre = _strdup (field);
            read_field (file_stream, field);
            dato->dni = _strdup (field);
            read_field (file_stream, field);
            dato->sexo = _strdup (field);
            read_field (file_stream, field);
            dato->fecha_nac = _strdup (field);
            read_field (file_stream, field);
            dato->telefono_movil = _strdup (field);
            read_field (file_stream, field);
            dato->correo_electronico = _strdup (field);
            read_field (file_stream, field);
            dato->direccion = _strdup (field);
            root = insertarArbol (root, (int(*)(const void*,const void*))compara_contacto, dato);
        }
        fclose (file_stream);
    }
    do {
        putchar ('\n');
        system (CLEAR_SCREEN);
        printf ("\tMENU\n");
        printf ("1.- Registrar\n");
        printf ("2.- Buscar\n");
        printf ("3.- Modificar\n");
        printf ("4.- Borrar\n");
        printf ("5.- Mostrar todos los datos\n");
        printf ("6.- Salir\n");
        printf ("Seleccione una opcion: ");
        fflush (stdout);
        do
            opcion_principal = _getch ();
        while (opcion_principal<'1' || opcion_principal>'6');
        printf ("%c\n\n", opcion_principal);
        if (root==NULL && opcion_principal!='1' && opcion_principal!='6')
        {
            pause_screen ("No hay registros.\n");
            continue;
        }
        if (opcion_principal<'5')
        {
            contacto.nombre = read_str ("Ingresa el nombre del contacto", field);
            dato = (Contacto*) buscarArbol (root, (int(*)(const void*,const void*))compara_contacto, &contacto);
            if (dato!=NULL)
            {
                putchar ('\n');
                imprime_contacto (dato, &contador);
            }
        }
        if (opcion_principal=='1' && dato!=NULL)
            printf ("El registro ya existe.\n");
        else if (opcion_principal>='2' && opcion_principal<='4' && dato==NULL)
            printf ("\nRegistro no encontrado.\n");
        else switch (opcion_principal)
        {
            case '1':
                dato = (Contacto*) malloc (sizeof (Contacto));
                dato->nombre = _strdup (field);
                dato->dni = read_string_line ("Ingresa el dni");
                dato->sexo = read_string_line ("Ingresa el sexo");
                dato->fecha_nac = read_string_line ("Ingresa la fecha de nacimiento");
                dato->telefono_movil = read_string_line ("Ingresa el telefono movil");
                dato->correo_electronico = read_string_line ("Ingresa el correo electronico");
                dato->direccion = read_string_line ("Ingrese la direccion");
                root = insertarArbol (root, (int(*)(const void*,const void*))compara_contacto, dato);
                printf ("\nRegistro agregado correctamente.\n");
                break;
            case '3':
                printf ("Menu de actualizacion de campos\n");
                printf ("1.- dni\n");
                printf ("2.- sexo\n");
                printf ("3.- fecha de nacimiento\n");
                printf ("4.- telefono movil\n");
                printf ("5.- correo electronico\n");
                printf ("6.- direccion\n");
                do {
                    subopcion = read_integer ("Selecciona el numero de campo a modificar");
                    if (subopcion<1 || subopcion>6);
                        printf ("Opcion correcta\n");
                } while (subopcion<1 || subopcion>6);
                switch (subopcion)
                {
                    case 1:
                        free (dato->dni);
                        dato->dni = read_string_line ("Ingresa el nuevo dni");
                        break;
                    case 2:
                        free (dato->sexo);
                        dato->sexo = read_string_line ("Ingresa el nuevo sexo");
                        break;
                    case 3:
                        free (dato->fecha_nac);
                        dato->fecha_nac = read_string_line ("Ingresa la nueva fecha de nacimiento");
                        break;
                    case 4:
                        free (dato->telefono_movil);
                        dato->telefono_movil = read_string_line ("Ingresa el nuevo telefono movil");
                        break;
                    case 5:
                        free (dato->correo_electronico);
                        dato->correo_electronico = read_string_line ("Ingresa el nuevo correo electronico");
                        break;
                    case 6:
                        free (dato->direccion);
                        dato->direccion = read_string_line ("Ingresa la nueva direccion");
                        break;
                }
                printf ("\nRegistro actualizado correctamente.\n");
                break;
            case '4':
                root = borrarArbol (root, (int(*)(const void*,const void*))compara_contacto, dato);
                free (dato->nombre);
                free (dato->dni);
                free (dato->sexo);
                free (dato->fecha_nac);
                free (dato->telefono_movil);
                free (dato->correo_electronico);
                free (dato->direccion);
                free (dato);
                printf ("Registro eliminado correctamente.\n");
                break;
            case '5':
                contador = 0;
                mostralArbol (root, &contador, (void(*)(void*,void*))imprime_contacto);
                printf ("Total de registros: %d.\n", contador);
                break;
        }
        if (opcion_principal!='6')
            pause_screen ("");
    } while (opcion_principal!='6');
    file_stream = fopen (path, "w");
    if (file_stream!=NULL)
    {
        arbol_inorder (root, file_stream, (void(*)(void*,void*))imprimir_en_archivo);
        fclose (file_stream);
    }
    return EXIT_SUCCESS;
}

arbolBinario insertarArbol (arbolBinario node, int (*compare)(const void*, const void*), void *dato)
{
    if (node==NULL)
    {
        node = (arbolBinario) malloc (sizeof (struct ParbolBinario));
        node->izquierda = NULL;
        node->derecha = NULL;
        node->dato = dato;
    } else {
        if (compare (dato, node->dato)<0)
            node->izquierda = insertarArbol (node->izquierda, compare, dato);
        else
            node->derecha = insertarArbol (node->derecha, compare, dato);
    }
    return node;
}

void* buscarArbol (arbolBinario node, int (*compare)(const void*, const void*), const void *dato)
{
    int comparison;
    if (node!=NULL)
    {
        comparison = compare (dato, node->dato);
        return comparison==0 ? node->dato : (comparison<0 ?
            buscarArbol (node->izquierda, compare, dato) :
            buscarArbol (node->derecha, compare, dato));
    }
    return NULL;
}

arbolBinario borrarArbol (arbolBinario node, int (*compare)(const void*, const void*), const void *dato)
{
    arbolBinario pivot;
    if (node!=NULL)
    {
        if (node->dato==dato)
        {
            if (node->izquierda==NULL)
                pivot = node->derecha;
            else if (node->derecha==NULL)
                pivot = node->izquierda;
            else
            {
                for (pivot=node->izquierda; pivot->derecha!=NULL; pivot=pivot->derecha);
                pivot->derecha = node->derecha;
                pivot = node->izquierda;
            }
            free (node);
            node = pivot;
        }
        else if (compare (dato, node->dato)<0)
            node->izquierda = borrarArbol (node->izquierda, compare, dato);
        else
            node->derecha = borrarArbol (node->derecha, compare, dato);
    }
    return node;
}

void mostralArbol (arbolBinario node, void *params, void (*func)(void*, void*))
{
    if (node!=NULL)
    {
        func (node->dato, params);
        mostralArbol (node->izquierda, params, func);
        mostralArbol (node->derecha, params, func);
    }
}

void arbol_inorder (arbolBinario node, void *params, void (*func)(void*, void*))
{
    if (node!=NULL)
    {
        arbol_inorder (node->izquierda, params, func);
        func (node->dato, params);
        arbol_inorder (node->derecha, params, func);
    }
}

int compara_contacto (const Contacto *contacto1, const Contacto *contacto2)
{
    return contacto1==contacto2 ? 0 : strcmp (contacto1->nombre, contacto2->nombre);
}

void imprime_contacto (Contacto *dato, int *contador)
{
    printf ("Nombre             : %s\n", dato->nombre);
    printf ("dni                 : %s\n", dato->dni);
    printf ("sexo                 : %s\n", dato->sexo);
    printf ("fecha de nacimiento  : %s\n", dato->fecha_nac);
    printf ("telefono movil    : %s\n", dato->telefono_movil);
    printf ("correo electronico: %s\n", dato->correo_electronico);
    printf ("direccion: %s\n", dato->direccion);
    putchar ('\n');
    (*contador)++;
}

void imprimir_en_archivo (Contacto *dato, FILE *file_stream)
{
    fprintf (file_stream, "%s\t", dato->nombre);
    fprintf (file_stream, "%s\t", dato->dni);
    fprintf (file_stream, "%s\t", dato->sexo);
    fprintf (file_stream, "%s\t", dato->fecha_nac);
    fprintf (file_stream, "%s\t", dato->telefono_movil);
    fprintf (file_stream, "%s\t", dato->correo_electronico);
    fprintf (file_stream, "%s\n", dato->direccion);
}

char* read_string_line (const char *mensaje)
{
    char string_line[255];
    (void) read_str (mensaje, string_line);
    return _strdup (string_line);
}

char* read_str (const char *mensaje, char *string_var)
{
    printf ("%s: ", mensaje);
    scanf ("%[^\r\n]", string_var);
    (void) getchar ();
    return string_var;
}

int read_integer (const char *mensaje)
{
    int integer_var;
    printf ("%s: ", mensaje);
    scanf ("%d", &integer_var);
    (void) getchar();
    return integer_var;
}

int read_field (FILE *file_stream, char *field)
{
    fscanf (file_stream, "%[^\t\n\r]", field);
    fgetc (file_stream);
    return feof (file_stream) == 0;
}

void pause_screen (const char *mensaje)
{
    printf ("%s\nPresiona una tecla para continuar . . . ", mensaje);
    fflush (stdout);
    _getch ();
}
