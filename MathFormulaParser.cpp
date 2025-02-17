/*
 * Autor: Artur Kregiel
 * Grupa: WTP 15:15    (wtorek parzysty godz 15:15)
 * Temat: Ewaluator wyrazen matematycznych
 */

#include <stdio.h>

//#include "MathFormulaEvaluator.hpp"
#include "MathFormulaParser.hpp"

/*

    Recursive descent parsing

    https://www.youtube.com/watch?v=SToUyjAsaFk

*/


// Definicje funkcji

namespace MathFormulaParser
{

    // parsuje wyra�enie
    MathExpression* ParseExpression(const char* expression, unsigned int& index)
    {
        MathExpression* result = ParseTerm(expression, index);

        if (result == NULL)
            return NULL;

        // dopóki jest co parsować
        while (expression[index] == '+' || expression[index] == '-')
        {
            char op = expression[index];
            index++;
            MathExpression* term = ParseTerm(expression, index);

            if (term == NULL)
                return NULL;

            if (op == '+')
                result = MathAdditionExpression::From(result, term);
            else
                result = MathSubtractionExpression::From(result, term);
        }

        return result;
    }

    // parsuje składnik
    MathExpression* ParseTerm(const char* expression, unsigned int& index)
    {
        MathExpression* result = ParseFactor(expression, index);

        if (result == NULL)
            return NULL;

        // dopóki jest co parsować
        while (expression[index] == '*' || expression[index] == '/')
        {
            char op = expression[index];
            index++;
            MathExpression* factor = ParseFactor(expression, index);

            if (factor == NULL)
                return NULL;

            if (op == '*')
                result = MathMultiplicationExpression::From(result, factor);
            else
                result = MathDivisionExpression::From(result, factor);
        }

        return result;
    }

    // parsuje czynnik
    MathExpression* ParseFactor(const char* expression, unsigned int& index)
    {
        MathExpression* result = ParsePower(expression, index);

        if (result == NULL)
            return NULL;

        // dopóki jest co parsowaź
        while (expression[index] == '^')
        {
            index++;
            MathExpression* power = ParsePower(expression, index);

            if (power == NULL)
                return NULL;

            result = MathExponationExpression::From(result, power);
        }

        return result;
    }

    // parsuje potęgę
    MathExpression* ParsePower(const char* expression, unsigned int& index)
    {
        MathExpression* primary = ParsePrimary(expression, index);
        
        if (primary == NULL)
            return NULL;

        return primary;
    }

    // parsuje wyrażenie podstawowe
    MathExpression* ParsePrimary(const char* expression, unsigned int& index)
    {
        MathExpression* result = NULL;

        if (expression[index] == '(')
        {
            index++;
            result = ParseExpression(expression, index);

            if (result == NULL)
                return NULL;

            if (expression[index] != ')')
                return NULL;
                
            index++;
        }
        else if (expression[index] >= '0' && expression[index] <= '9')
        {
            // parsowanie liczby (stałej)
            double value = 0;
            while (expression[index] >= '0' && expression[index] <= '9')
            {
                value = value * 10 + (expression[index] - '0');
                index++;
            }
            if (expression[index] == '.')   // jesli ma część ułamkową
            {
                index++;
                double multiplier = 0.1;
                while (expression[index] >= '0' && expression[index] <= '9')
                {
                    value += (expression[index] - '0') * multiplier;
                    multiplier *= 0.1;
                    index++;
                }
            }
            result = MathConstant::From(value);
        }
        else if (expression[index] == 'x')
        {
            // parsowanie zmiennej
            result = new MathVariable();
            index++;
        }
        else if (expression[index] == 'e')
        {
            // parsowanie liczby e
            result = new MathEulerNumber();
            index++;
        }
        else if (expression[index] == 's' && expression[index + 1] == 'i' && expression[index + 2] == 'n')
        {
            // parsowanie funkcji sinus
            index += 3;
            MathExpression* arg = ParsePrimary(expression, index);

            if (arg == NULL)
                return NULL;

            result = MathSinus::From(arg);
        }
        else if (expression[index] == 'c' && expression[index + 1] == 'o' && expression[index + 2] == 's')
        {
            // parsowanie funkcji cosinus
            index += 3;
            MathExpression* arg = ParsePrimary(expression, index);

            if (arg == NULL)
                return NULL;

            result = MathCosinus::From(arg);
        }
        else if (expression[index] == 't' && expression[index + 1] == 'a' && expression[index + 2] == 'n')
        {
            // parsowanie funkcji tangens
            index += 3;
            MathExpression* arg = ParsePrimary(expression, index);

            if (arg == NULL)
                return NULL;

            result = MathTangent::From(arg);
        }
        else if (expression[index] == 'c' && expression[index + 1] == 'o' && expression[index + 2] == 't')
        {
            // parsowanie funkcji cotangens
            index += 3;
            MathExpression* arg = ParsePrimary(expression, index);

            if (arg == NULL)
                return NULL;

            result = MathCotangent::From(arg);
        }
        else if (expression[index] == 'l' && expression[index + 1] == 'n')
        {
            // parsowanie funkcji logarytmu naturalnego
            index += 2;
            MathExpression* arg = ParsePrimary(expression, index);

            if (arg == NULL)
                return NULL;

            result = MathLogarithmNatural::From(arg);
        }
        else if (expression[index] == 'l' && expression[index + 1] == 'o' && expression[index + 2] == 'g')
        {
            // parsowanie funkcji logarytmu (o podstawie 10)
            index += 3;
            MathExpression* arg = ParsePrimary(expression, index);

            if (arg == NULL)
                return NULL;

            result = MathLogarithm10::From(arg);
        }
        else if (expression[index] == '-')
        {
            // parsowanie negacji
            index++;
            MathExpression* arg = ParseFactor(expression, index);

            if (arg == NULL)
                return NULL;

            result = MathMultiplicationExpression::From(MathConstant::From(-1), arg);
        }
        else
        {
            // nieznany znak
            result = NULL;
        }

        return result;
    }

}