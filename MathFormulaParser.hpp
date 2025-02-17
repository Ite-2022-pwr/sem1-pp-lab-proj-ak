#pragma once

/*
 * Autor: Artur Kregiel
 * Grupa: WTP 15:15    (wtorek parzysty godz 15:15)
 * Temat: Ewaluator wyrazen matematycznych
 */

#include "MathFormulaEvaluator.hpp"

/*

    Recursive descent parsing

    https://www.youtube.com/watch?v=SToUyjAsaFk

*/


// prototypy funkcji

namespace MathFormulaParser
{
    using namespace MathFormulaEvaluator;

    // parsuje wyrażenie
    MathExpression* ParseExpression(const char* expression, unsigned int& index);

    // parsuje składnik
    MathExpression* ParseTerm(const char* expression, unsigned int& index);

    // parsuje czynnik
    MathExpression* ParseFactor(const char* expression, unsigned int& index);

    // parsuje potęgę
    MathExpression* ParsePower(const char* expression, unsigned int& index);

    // parsuje podstowe wyrażenie
    MathExpression* ParsePrimary(const char* expression, unsigned int& index);
}