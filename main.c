#include <stdio.h>  // Uso general
#include <string.h> // Uso para strcpy, strlen
#include <ctype.h> // Uso para toupper, isalpha
#include <math.h> // Uso para pow (potencia)

#define BASE_BINARIA 2

/*
    Esta funcion tiene como objetivo:
        - Imprimir la tabla
        - Imprimir la FND y la FNC si existe
*/
void imprimirResultadosYFormas(int cantidadVariables, int cantidadCeldas, char variablesIngresadas[], int tabla[][cantidadCeldas], int resultadosFinales[], char expresionIngresada[])
{
    printf("\n-- RESULTADOS --\n\n");

    // Mostrar los literales a modo "grafico" uno al lado del otro
    printf("-- Tabla de verdad--\n");
    printf("--------------------------------------------\n");
    for (int v = 0; v < cantidadVariables; v++)
    {
        printf("%c   ", variablesIngresadas[v]);
    }
    printf("| %s\n", expresionIngresada);
    printf("--------------------------------------------\n");

    // Imprimir los valores de los literales generados desde la funcion crearTabla
    // Imprimir los resultados de la exresion
    for (int c = 0; c < cantidadCeldas; c++)
    {
        for (int v = 0; v < cantidadVariables; v++)
        {
            printf("%i   ", tabla[v][c]);
        }
        printf("\t    %i\n", resultadosFinales[c]);
        printf("--------------------------------------------\n");
    }
    printf("\n\n");
    // Generar FND (suma de productos) para filas donde resultadoFinal == 1
    printf("-- Formas --\n");
    printf("FND (Forma Normal Disyuntiva): ");
    int primera = 1;
    for (int c = 0; c < cantidadCeldas; c++)
    {
        if (resultadosFinales[c] == 1)
        {
            // Si no es el primer termino, podemos poner el operador
            if (!primera)
            {
                printf(" + ");
            }

            printf("(");
            for (int v = 0; v < cantidadVariables; v++)
            {
                if (v > 0)
                {
                    printf("*");
                }

                if (tabla[v][c] == 1)
                {
                    printf("%c", variablesIngresadas[v]);
                }
                else
                {
                    printf("!%c", variablesIngresadas[v]);
                }
            }
            printf(")");

            primera = 0;
        }
    }
    if (primera == 1)
    {
        printf("No existe FND.\n");
    }

    // Generar FNC (producto de sumas) para filas donde resultadoFinal == 0
    printf("\nFNC (Forma Normal Conjuntiva): ");
    primera = 1;
    for (int c = 0; c < cantidadCeldas; c++)
    {
        if (resultadosFinales[c] == 0)
        {
            if (!primera)
                printf(" * ");

            printf("(");
            for (int v = 0; v < cantidadVariables; v++)
            {
                if (v > 0)
                    printf("+");
                if (tabla[v][c] == 0)
                    printf("%c", variablesIngresadas[v]);
                else
                    printf("!%c", variablesIngresadas[v]);
            }
            printf(")");

            primera = 0;
        }
    }
    if (primera == 1)
    {
        printf("No existe FNC.\n");
    }
    printf("\n");
}

/*
    Esta funcion tiene como objetivo:
        - Recibir la expresion de un termino
        - Interpretar la postfija
        - Devolver el resultado
        - Ejemplo de expresion que recibe: 1 1 0 + *
*/
int evaluarPostfija(char expresion[])
{
    int resultados[64];
    int posicionResultados = 0;

    for (int i = 0; i < strlen(expresion); i++)
    {
        char c = expresion[i];

        // Convertir caracter a digito
        if (c == '0' || c == '1')
        {
            resultados[posicionResultados] = c - '0';
            posicionResultados++;
        }
        else if (c == '!')
        {
            // Obtener complemento
            posicionResultados--;
            int valorA = resultados[posicionResultados];
            resultados[posicionResultados] = !valorA;
            posicionResultados++;
        }
        else if (c == '+')
        {
            // Obtener suma
            posicionResultados--; 
            int valorA = resultados[posicionResultados];
            posicionResultados--; 
            int valorB = resultados[posicionResultados];
            resultados[posicionResultados] = valorA || valorB;
            posicionResultados++;
        }
        else if (c == '*')
        {
            // Obtener producto
            posicionResultados--;
            int valorA = resultados[posicionResultados];
            posicionResultados--; 
            int valorB = resultados[posicionResultados];
            resultados[posicionResultados] = valorA && valorB;
            posicionResultados++;
        }
    }
    return resultados[0];
}

/*
    Esta funcion tiene como objetivo:
        - Recibe un termino en notacion postfija
        - Cambia el valor letra por numero correspondiente (x x y + * -- 1 1 0 + *)
*/
void reemplazarVariablesPorValores(char postfija[], char variables[], int valores[], int cantidadVariables, char resultado[])
{
    int i = 0;
    int j = 0;

    while (postfija[i] != '\0')
    {
        char c = postfija[i];

        if (isalpha(c))
        {
            for (int k = 0; k < cantidadVariables; k++)
            {
                if (variables[k] == c)
                {
                    // Convertir numero a caracter
                    resultado[j++] = valores[k] + '0';
                    break;
                }
            }
        }
        else
        {
            resultado[j++] = c;
        }

        i++;
    }

    resultado[j] = '\0';
}

/*
    Esta funcion tiene como objetivo:
        - Determinar el orden de operador (jerarquia, primero parentesis, complemento, multiplicacion, suma)
        - Hacer las diferentes operaciones: SUMA, MULTIPLICACION Y COMPLEMENTO
        - Almacenar los resultados en un array
        - Mostrar los resultados
*/
void calcularExpresion(int cantidadVariables, int cantidadCeldas, int tabla[][cantidadCeldas], char variablesIngresadas[], int cantidadTerminos, char terminos[][32], char expresionIngresada[])
{
    char resultadosPostfijos[cantidadTerminos][32];

    // Recorremos todos los terminos y convertimos a cada uno en postfijo
    for (int t = 0; t < cantidadTerminos; t++)
    {
        int longitudTermino = strlen(terminos[t]);
        char operadores[longitudTermino];
        char salida[longitudTermino];
        memset(operadores, '\0', longitudTermino);
        memset(salida, '\0', longitudTermino);

        int posicionSalida = 0;
        int posicionOperadores = 0;

        for (int e = 0; e < longitudTermino; e++)
        {
            char caracter = terminos[t][e];
            if (caracter != '\n' && caracter != ' ' && caracter != '\0')
            {
                if (isalpha(caracter))
                {
                    salida[posicionSalida++] = caracter;
                }
                else if (caracter == '(')
                {
                    operadores[posicionOperadores++] = caracter;
                }
                else if (caracter == ')')
                {
                    while (posicionOperadores > 0 && operadores[posicionOperadores - 1] != '(')
                    {
                        salida[posicionSalida++] = operadores[--posicionOperadores];
                    }

                    if (posicionOperadores > 0 && operadores[posicionOperadores - 1] == '(')
                    {
                        posicionOperadores--;
                    }
                }
                else
                {
                    int presedenciaActual = (caracter == '+') ? 1 : (caracter == '*') ? 2 : (caracter == '!') ? 3 : 0;

                    while (posicionOperadores > 0)
                    {
                        char operadorTope = operadores[posicionOperadores - 1];
                        int presedenciaTope = (operadorTope == '+') ? 1 : (operadorTope == '*') ? 2 : (operadorTope == '!') ? 3 : 0;

                        if (operadorTope == '(' || presedenciaActual >= presedenciaTope)
                            break;

                        salida[posicionSalida++] = operadores[--posicionOperadores];
                    }

                    operadores[posicionOperadores++] = caracter;
                }
            }
        }

        while (posicionOperadores > 0)
        {
            salida[posicionSalida++] = operadores[--posicionOperadores];
        }

        salida[posicionSalida] = '\0';
        strcpy(resultadosPostfijos[t], salida);
    }

    // Evaluamos cada término por fila
    int resultadosFinales[cantidadCeldas];
    for (int c = 0; c < cantidadCeldas; c++)
    {
        int valoresFila[cantidadVariables];
        for (int v = 0; v < cantidadVariables; v++)
        {
            valoresFila[v] = tabla[v][c];
        }

        int resultadoTerminos[cantidadTerminos];

        for (int t = 0; t < cantidadTerminos; t++)
        {
            // Array donde se almacena la expresion reemplazada, es decir, las letras/literales pasana a ser numeros
            char expresionConValores[64];
            reemplazarVariablesPorValores(resultadosPostfijos[t], variablesIngresadas, valoresFila, cantidadVariables, expresionConValores);
            
            // Vamos almacenando el resultado de cada termino
            resultadoTerminos[t] = evaluarPostfija(expresionConValores);
        }

        // Suma final de los terminos (OR)
        resultadosFinales[c] = resultadoTerminos[0];
        for (int i = 1; i < cantidadTerminos; i++)
        {
            resultadosFinales[c] = resultadosFinales[c] || resultadoTerminos[i];
        }
    }

    imprimirResultadosYFormas(cantidadVariables, cantidadCeldas, variablesIngresadas, tabla, resultadosFinales, expresionIngresada);
}


/*
    Esta funcion tiene como objetivo:
        - Ver si la expresion esta apta para ser calculada
        - Determinar la cantidad y los terminos
*/
void validarExpresion(int cantidadVariables, int cantidadCeldas, int tabla[][cantidadCeldas], char variablesIngresadas[cantidadVariables], char expresionIngresada[64])
{
    // Validar si las variables/literales de una expresion se encuentran definidias
    int error = 0;
    for (int i = 0; i < strlen(expresionIngresada); i++)
    {
        // Verificamos si es una letra (variable/literal y no es un operador + o *, etc.)
        if (isalpha(expresionIngresada[i]))
        {
            // Si entra aca adentro quiere decir que lo de la expresione es una letra
            // X - Compararla con todas las que esten almacenadas (X, Y, N)
            for (int j = 0; j < cantidadVariables; j++)
            {
                if (toupper(expresionIngresada[i]) == toupper(variablesIngresadas[j]))
                {
                    // Si ya la encontro, el error lo desactivamos
                    error = 0;
                    break;
                }
                else
                {
                    // Si vemos que no la va encontrando, entonces el error lo activamos
                    error = 1;
                }
            }
        }
    }

    if (error == 0)
    {
        int longitud = strlen(expresionIngresada);

        // FUNCION: DETERMINAR LA CANTIDAD DE TERMINOS (LO INICIAMOS EN 1, PORQUE AL MENOS 1 DEBE TENER)
        int cantidadTerminos = 1;
        int contadorParentesis = 0;
        for (int i = 0; i < longitud; i++)
        {
            char c = expresionIngresada[i];

            if (c == '(')
            {
                contadorParentesis++;
            }
            else if (c == ')')
            {
                contadorParentesis--;
            }

            if (c == '+' && contadorParentesis == 0)
            {
                cantidadTerminos++;
            }
        }

        // FUNCION: SEPARAR LA EXPRESION EN LA CANTIDAD DE TERMINOS QUE PREVIAMENTE OBTUVIMOS
        char terminos[cantidadTerminos][32];
        int numeroTermino = 0;
        // Rellenar con espacios vacios a todo el contenido del termino, para que no quede basura
        for (int i = 0; i < cantidadTerminos; i++)
        {
            memset(terminos[i], '\0', 32);
        }
        int posicionTermino = 0;
        for (int i = 0; i < longitud; i++)
        {
            char c = expresionIngresada[i];

            if (c == '(')
            {
                contadorParentesis++;
            }
            else if (c == ')')
            {
                contadorParentesis--;
            }

            if (c == '+' && contadorParentesis == 0)
            {
                posicionTermino = 0;
                numeroTermino++;
            }

            if (c != '+' && c != ' ')
            {
                terminos[numeroTermino][posicionTermino] = expresionIngresada[i];
                posicionTermino++;
            }

            if (c == '+' && contadorParentesis > 0)
            {
                terminos[numeroTermino][posicionTermino] = expresionIngresada[i];
                posicionTermino++;
            }
        }

        calcularExpresion(cantidadVariables, cantidadCeldas, tabla, variablesIngresadas, cantidadTerminos, terminos, expresionIngresada);
    }
    else
    {
        printf("Error: La expresion posee literales no definidas.");
    }
}

/*
    Esta funcion tiene como objetivo:
        - Determinar la dimension de la tabla
        - Rellenar los valores de la tabla
*/
void crearTabla(int cantidadVariables)
{
    char variables[cantidadVariables];
    for (int i = 0; i < cantidadVariables; i++)
    {
        printf("Ingrese una letra para la variable N%i: ", i + 1);
        scanf(" %c", &variables[i]);
    }

    int cantidadCeldas = pow(BASE_BINARIA, cantidadVariables);
    int modoRelleno = 1;
    int contador = 1;
    int tabla[cantidadVariables][cantidadCeldas];
    int dividendo = cantidadCeldas;

    // Almacenar
    for (int columna = 0; columna < cantidadVariables; columna++)
    {
        int intervalo = dividendo / BASE_BINARIA;
        for (int fila = 0; fila < cantidadCeldas; fila++)
        {
            if (contador > intervalo)
            {
                contador = 1;
                modoRelleno = !modoRelleno;
            }

            if (modoRelleno == 1)
            {
                tabla[columna][fila] = 1;
                contador++;
            }
            else
            {
                tabla[columna][fila] = 0;
                contador++;
            }
        }
        contador = 1;
        modoRelleno = 1;
        dividendo = intervalo;
    }

    char expresion[64];
    printf("Ingrese una expresion: ");
    // Limpiamos, porque anteriormente utilizamos scanf para las variables.
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    fgets(expresion, 64, stdin);

    validarExpresion(cantidadVariables, cantidadCeldas, tabla, variables, expresion);
}

void main()
{
    int cantidadVariables;
    printf("Bienvenido\n\n");
    printf("-- CALCULADORA DE FND Y FNC --\n\n");
    do
    {
        printf("Ingrese la cantidad de variables: ");
        scanf("%i", &cantidadVariables);
        if (cantidadVariables <= 0)
        {
            printf("Error: La cantidad de variables debe ser mayor que 0.");
        }
    } while (cantidadVariables <= 0);

    crearTabla(cantidadVariables);
}