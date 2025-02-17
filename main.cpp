/*
 * Autor: Artur Kregiel
 * Grupa: WTP 15:15    (wtorek parzysty godz 15:15)
 * Temat: Ewaluator wyrazen matematycznych
 */

#include <stdio.h>
#include <string.h>

#include "MathFormulaEvaluator.hpp"
#include "MathFormulaParser.hpp"

using namespace MathFormulaEvaluator;
using namespace MathFormulaParser;

#define MAX_EXPRESSION_LENGTH 256

MathExpression* Expression = NULL;
MathExpression* Derivative1 = NULL;
MathExpression* Derivative2 = NULL;

MathExpression* readExpression()
{
    unsigned int index = 0;
    char expression[MAX_EXPRESSION_LENGTH] = { 0 };

    printf("Podaj wyrazenie matematyczne: ");
    fgets(expression, MAX_EXPRESSION_LENGTH, stdin);
    expression[strlen(expression) - 1] = '\0';

    MathExpression* parsedExpression = ParseExpression(expression, index);
    //printf("index: %u\n", index);

    if (parsedExpression == NULL || expression[index] != '\0')
    {
        fprintf(stderr, "BLAD podczas parsowania blisko znaku '%c' na pozycji %u\n", expression[index], index);
        return NULL;
    }

    return parsedExpression;
}

void printExpression()
{
    printf("f(x) = ");
    Expression->Print();
    printf("\n");

    printf("f'(x) = ");
    Derivative1->Print();
    printf("\n");

    printf("f\"(x) = ");
    Derivative2->Print();
    printf("\n");
}

void evaluateExpression()
{
    double x;
    printf("Podaj, dla jakiej wartosci x obliczyc wartosc wyrazenia: ");
    scanf("%lf", &x);
    printf("f(x) = f(%g) = %.3lf\n", x, Expression->CalculateFor(x));
    printf("f'(x) = f'(%.g) = %.3lf\n", x, Derivative1->CalculateFor(x));
    printf("f\"(x) = f\"(%.g) = %.3lf\n", x, Derivative2->CalculateFor(x));
}

void evalRange(double low, double high, double jump)
{
	printf("\n");
	
	printExpression();
	
	printf("\n");
	printf("|%16c|%16s|%16s|%16s|\n", 'x', "f(x)", "f'(x)", "f\"(x)");
	printf("|-------------------------------------------------------------------|\n");
	for (double x = low; x <= high; x += jump)
	{
		double valF = Expression->CalculateFor(x);		// wartosc funkcji
		double valD1 = Derivative1->CalculateFor(x);	// wartosc pierwszej pochodnej
		double valD2 = Derivative2->CalculateFor(x);	// wartosc drugiej pochodnej
		
		printf("|%16.2f|%16.2f|%16.2f|%16.2f|\n", x, valF, valD1, valD2);
	}
}

void clear()
{
    if (Expression != NULL)
    {
        delete Expression;
        Expression = NULL;
    }

    if (Derivative1 != NULL)
    {
        delete Derivative1;
        Derivative1 = NULL;
    }

    if (Derivative2 != NULL)
    {
        delete Derivative2;
        Derivative2 = NULL;
    }
}

int main()
{
    printf("Autor: Artur Kregiel (WTP 15:15)\n\n");
    //const char* expression1 = "-e^(x^3+2)+4*-(x^5+78.89-67*x)-sin(x^2+4/x-3)"; //"x^2.5+3*x+5";

    int choice = 0;
    do
    {
        printf("\n\nMENU:\n");
        printf("0. Wyjscie\n");
        printf("1. Wczytaj wyrazenie\n");
        printf("2. Wyswietl wyrazenie\n");
        printf("3. Oblicz wartosc wyrazenia\n");
        printf("4. Oblicz wartosci na przedziale [a;b] z podanym skokiem\n");

        printf("Wybor: ");
        scanf("%d", &choice);
        getchar();
        printf("\n\n");

        switch (choice)
        {
        case 0:
            clear();
            break;
        case 1:
            clear();
            Expression = readExpression();
            if (Expression != NULL)
            {
                Derivative1 = Expression->Derivative();
                Derivative2 = Derivative1->Derivative();
            }
            break;
        case 2:
            if (Expression != NULL)
            {
                printExpression();
            }
            else
            {
                printf("Nie wczytano wyrazenia!\n");
            }
            break;
        case 3:
            if (Expression != NULL)
            {
                evaluateExpression();
            }
            else
            {
                printf("Nie wczytano wyrazenia!\n");
            }
            break;
        case 4:
        	if (Expression != NULL)
            {
                double a, b, jmp;
                printf("podaj poczatek przedzialu: ");
                scanf("%lf", &a);
                printf("podaj koniec przedzialu: ");
                scanf("%lf", &b);
                printf("podaj skok: ");
                scanf("%lf", &jmp);
                printf("\n");
                
                evalRange(a, b, jmp);
            }
            else
            {
                printf("Nie wczytano wyrazenia!\n");
            }
            break;
        default:
            printf("Niepoprawna opcja! Tylko 0-3\n");
            break;
        }
    } while (choice != 0);
    

    printf("\n\n\n");

    //printf("Ilosc utworzonych dynamicznie obiektow w pamieci: %d\n", MathExpression::GetObjectCount());

    //printf("Usuwanie obiektow...\n");

    //printf("Pozostalo obiektew w pamieci: %d\n", MathExpression::GetObjectCount());

    return 0;
}

/*

Przykładowe wyrażenia

x^2.5+3*x+5
-e^(x^3+2)+4*-(x^5+78.89-67*x)-sin(x^2+4/x-3)
(x^2.5-4*x)/(3*x+5)
4*x^2+5*x-4+0*69*x-sin(3)+e

*/