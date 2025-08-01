/*
 * Autor: Artur Kregiel
 * Grupa: WTP 15:15    (wtorek parzysty godz 15:15)
 * Temat: Ewaluator wyrazen matematycznych
 */

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>

#include "MathFormulaEvaluator.hpp"

// Definicje metod

namespace MathFormulaEvaluator
{

    unsigned int MathExpression::objectCount = 0;

    /********************/
    /*** MathVariable ***/
    /********************/

    MathExpression* MathVariable::Derivative()
    {
        return MathConstant::From(1);
    }

    /**********************/
    /*** MathPolynomial ***/
    /**********************/

    MathPolynomial::MathPolynomial(double coefficient, double exponent) : MathExpression()
    {
        type = MathExpressionType_Polynomial;
        this->coefficient = coefficient;
        this->exponent = exponent;
    }

    MathPolynomial::MathPolynomial(const MathPolynomial& obj) : MathExpression()
    {
        type = obj.type;
        coefficient = obj.coefficient;
        exponent = obj.exponent;
    }

    MathPolynomial::~MathPolynomial()
    {
        #ifdef _MATH_EVALUATOR_DEBUG_VERBOSE
        printf("[[ MathPolynomial destructor ]]\n");
        #endif
    }

    MathExpression* MathPolynomial::From(double coefficient, double exponent)
    {
        if (coefficient == 0)
            return MathConstant::From(0);
        else if (exponent == 0)
            return MathConstant::From(coefficient);
        else
            return new MathPolynomial(coefficient, exponent);
    }

    double MathPolynomial::CalculateFor(double x)
    {
        return coefficient * pow(x, exponent);
    }

    void MathPolynomial::Print()
    {
        printf("(");
        if (coefficient < 0)
        {
            if (coefficient == (int)coefficient)
                printf("(%d)*", (int)coefficient);
            else
                printf("(%.3f)*", coefficient);
        }
        else if (coefficient != 1)
        {
            if (coefficient == (int)coefficient)
                printf("%d*", (int)coefficient);
            else
                printf("%.3f*", coefficient);
        }

        if (exponent == 1)
        {
            printf("x");
        }
        else if (exponent != 0)
        {
            if (exponent == (int)exponent)
                printf("x^%d", (int)exponent);
            else
                printf("x^%.3f", exponent);
        }
        printf(")");
    }

    MathExpression* MathPolynomial::Copy()
    {
        return new MathPolynomial(*this);
    }

    MathExpression* MathPolynomial::Derivative()
    {
        if (exponent == 1)
            return MathConstant::From(coefficient);
        else
            return MathPolynomial::From(coefficient * exponent, exponent - 1);
    }

    /********************/
    /*** MathConstant ***/
    /********************/

    MathConstant::MathConstant(double val) : MathExpression()
    {
        type = MathExpressionType_Constant;
        value = val;
    }

    MathConstant::MathConstant(const MathConstant& obj) : MathExpression()
    {
        type = obj.type;
        value = obj.value;
    }

    void MathConstant::Print()
    {
        if (value < 0)
        {
            if (value == (int)value)
                printf("(%d)", (int)value);
            else
                printf("(%.3f)", value);
        }
        else
        {
            if (value == (int)value)
                printf("%d", (int)value);
            else
                printf("%.3f", value);
        }
    }

    /***********************/
    /*** MathEulerNumber ***/
    /***********************/

    MathEulerNumber::MathEulerNumber() : MathConstant(exp(1))
    {
        type = MathExpressionType_EulerNumber;
    }

    MathEulerNumber::MathEulerNumber(const MathEulerNumber& obj) : MathConstant(obj)
    {
        type = obj.type;
    }

    MathEulerNumber::~MathEulerNumber()
    {
        #ifdef _MATH_EVALUATOR_DEBUG_VERBOSE
        printf("[[ MathEulerNumber destructor ]]\n");
        #endif
    }

    void MathEulerNumber::Print()
    {
        printf("e");
    }

    MathExpression* MathEulerNumber::Copy()
    {
        return new MathEulerNumber(*this);
    }

    /********************************/
    /*** MathArithmeticExpression ***/
    /********************************/

    MathArithmeticExpression::MathArithmeticExpression(MathExpression* left, MathExpression* right, char op) : MathExpression()
    {
        type = MathExpressionType_Arithmetic;
        this->left = left;
        this->right = right;
        this->op = op;
    }

    MathArithmeticExpression::MathArithmeticExpression(const MathArithmeticExpression& obj)
    {
        type = MathExpressionType_Arithmetic;
        left = obj.left->Copy();
        right = obj.right->Copy();
        op = obj.op;
    }

    MathArithmeticExpression::~MathArithmeticExpression()
    {
        #ifdef _MATH_EVALUATOR_DEBUG_VERBOSE
        printf("[[ MathArithmeticExpression destructor (oper: '%c') ]]\n", op);
        #endif
        delete left;
        delete right;
    }

    char MathArithmeticExpression::GetOperator()
    {
        return op;
    }

    double MathArithmeticExpression::CalculateFor(double x)
    {
        switch (op)
        {
            case '+':
                return left->CalculateFor(x) + right->CalculateFor(x);
            case '-':
                return left->CalculateFor(x) - right->CalculateFor(x);
            case '*':
                return left->CalculateFor(x) * right->CalculateFor(x);
            case '/':
                return left->CalculateFor(x) / right->CalculateFor(x);
            case '^':
                return pow(left->CalculateFor(x), right->CalculateFor(x));
            default:
                return 0;
        }
    }

    void MathArithmeticExpression::Print()
    {
        if (left->GetExpressionType() == this->type)
        {
            MathArithmeticExpression* leftOperand = (MathArithmeticExpression*)this->left;

            if (MathExpression::GetOperatorPriority(leftOperand->GetOperator()) <= MathExpression::GetOperatorPriority(this->op))
            {
                printf("(");
                left->Print();
                printf(")");
            }
            else
            {
                left->Print();
            }
        }
        else
        {
            left->Print();
        }

        printf("%c", this->op);

        if (right->GetExpressionType() == this->type)
        {
            MathArithmeticExpression* rightOperand = (MathArithmeticExpression*)this->right;

            if (MathExpression::GetOperatorPriority(rightOperand->GetOperator()) <= MathExpression::GetOperatorPriority(this->op) || this->op == '-')
            {
                printf("(");
                right->Print();
                printf(")");
            }
            else
            {
                right->Print();
            }
        }
        else
        {
            right->Print();
        }
    }

    /********************************/
    /*** MathExponationExpression ***/
    /********************************/

    MathExponationExpression::MathExponationExpression(MathExpression* left, MathExpression* right) : MathArithmeticExpression(left, right, '^') {}

    MathExponationExpression::MathExponationExpression(const MathExponationExpression& obj) : MathArithmeticExpression(obj) {}

    MathExpression* MathExponationExpression::Derivative()
    {
        MathExpressionType leftOperandType = left->GetExpressionType();

        MathExpressionType rightOperandType = right->GetExpressionType();

        if (leftOperandType == MathExpressionType_Constant && rightOperandType == MathExpressionType_Constant)
        {
            // C ^ C
            return MathConstant::From(0);
        }
        else if (leftOperandType == MathExpressionType_Constant)
        {
            // C ^ W
            return MathMultiplicationExpression::From(
                this->Copy(),
                MathMultiplicationExpression::From(
                    MathLogarithmNatural::From(left->Copy()),
                    right->Derivative()
                )
            );
        }
        else if (leftOperandType == MathExpressionType_EulerNumber)
        {
            // e ^ W
            return MathMultiplicationExpression::From(
                this->Copy(),
                right->Derivative()
            );
        }
        else if (rightOperandType == MathExpressionType_Constant)
        {
            // W ^ C
            return MathMultiplicationExpression::From(
                MathMultiplicationExpression::From(
                    right->Copy(),
                    new MathExponationExpression(
                        left->Copy(),
                        MathConstant::From(right->CalculateFor(0) - 1)
                    )
                ),
                left->Derivative()
            );
        }
        else
        {
            // W ^ W
            // W ^ V == e ^ (V * ln(W))
            MathExponationExpression temp(
                new MathEulerNumber(),
                MathMultiplicationExpression::From(
                    right->Copy(),
                    MathLogarithmNatural::From(left->Copy())
                )
            );
            
            return temp.Derivative();

        }
    }

    MathExpression* MathExponationExpression::Copy()
    {
        return new MathExponationExpression(*this);
    }

    MathExpression* MathExponationExpression::From(MathExpression* left, MathExpression* right)
    {
        MathExpression* result;

        if (left->GetExpressionType() == MathExpressionType_Constant && right->GetExpressionType() == MathExpressionType_Constant)
        {
            result = MathConstant::From(pow(left->CalculateFor(0), right->CalculateFor(0)));
            delete right;
            delete left;
        }
        else if (left->GetExpressionType() == MathExpressionType_Variable && right->GetExpressionType() == MathExpressionType_Constant)
        {
            if (right->CalculateFor(0) == 0)
            {
                result = MathConstant::From(1);
                delete right;
                delete left;
            }
            else if (right->CalculateFor(0) == 1)
            {
                delete right;
                return left;
            }
            else
            {
                result = MathPolynomial::From(1, right->CalculateFor(0));
                delete right;
                delete left;
            }
        }
        else if (left->GetExpressionType() == MathExpressionType_Constant && right->GetExpressionType() == MathExpressionType_Variable)
        {
            if (left->CalculateFor(0) == 0)
            {
                result = MathConstant::From(0);
                delete right;
                delete left;
            }
            else if (left->CalculateFor(0) == 1)
            {
                result = MathConstant::From(1);
                delete right;
                delete left;
            }
        }
        else return new MathExponationExpression(left, right);

        return result;
    }

    /******************************/
    /*** MathAdditionExpression ***/
    /******************************/

    MathAdditionExpression::MathAdditionExpression(MathExpression* left, MathExpression* right) : MathArithmeticExpression(left, right, '+') {}

    MathAdditionExpression::MathAdditionExpression(const MathAdditionExpression& obj) : MathArithmeticExpression(obj) {}

    MathExpression* MathAdditionExpression::Derivative()
    {
        MathExpressionType leftOperandType = left->GetExpressionType();

        MathExpressionType rightOperandType = right->GetExpressionType();

        if (leftOperandType == MathExpressionType_Constant && rightOperandType == MathExpressionType_Constant)
        {
            return MathConstant::From(0);
        }
        else if (leftOperandType == MathExpressionType_Constant)
        {
            return right->Derivative();
        }
        else if (rightOperandType == MathExpressionType_Constant)
        {
            return left->Derivative();
        }
        else
        {
            return MathAdditionExpression::From(left->Derivative(), right->Derivative());
        }
    }

    MathExpression* MathAdditionExpression::Copy()
    {
        return new MathAdditionExpression(*this);
    }

    MathExpression* MathAdditionExpression::From(MathExpression* left, MathExpression* right)
    {
        MathExpression* result;

        if (left->GetExpressionType() == MathExpressionType_Constant && right->GetExpressionType() == MathExpressionType_Constant)
        {
            result = MathConstant::From(left->CalculateFor(0) + right->CalculateFor(0));
            delete right;
            delete left;
        }
        else if (left->GetExpressionType() == MathExpressionType_Constant)
        {
            if (left->CalculateFor(0) == 0)
            {
                delete left;
                return right;
            }
            else
            {
                return new MathAdditionExpression(left, right);
            }
        }
        else if (right->GetExpressionType() == MathExpressionType_Constant)
        {
            if (right->CalculateFor(0) == 0)
            {
                delete right;
                return left;
            }
            else
            {
                return new MathAdditionExpression(left, right);
            }
        }
        else if (left->GetExpressionType() == MathExpressionType_Polynomial && right->GetExpressionType() == MathExpressionType_Polynomial)
        {
            MathPolynomial* leftPolynomial = (MathPolynomial*)left;
            MathPolynomial* rightPolynomial = (MathPolynomial*)right;

            if (leftPolynomial->GetExponent() == rightPolynomial->GetExponent())
            {
                result = MathPolynomial::From(leftPolynomial->GetCoefficient() + rightPolynomial->GetCoefficient(), leftPolynomial->GetExponent());
                delete right;
                delete left;
            }
            else
            {
                return new MathAdditionExpression(left, right);
            }
        }
        else if (left->GetExpressionType() == MathExpressionType_Variable && right->GetExpressionType() == MathExpressionType_Variable)
        {
            result = MathPolynomial::From(2, 1);
            delete right;
            delete left;
        }
        else return new MathAdditionExpression(left, right);

        return result;
    }

    /*********************************/
    /*** MathSubtractionExpression ***/
    /*********************************/

    MathSubtractionExpression::MathSubtractionExpression(MathExpression* left, MathExpression* right) : MathArithmeticExpression(left, right, '-') {}

    MathSubtractionExpression::MathSubtractionExpression(const MathSubtractionExpression& obj) : MathArithmeticExpression(obj) {}

    MathExpression* MathSubtractionExpression::Derivative()
    {
        MathExpressionType leftOperandType = left->GetExpressionType();

        MathExpressionType rightOperandType = right->GetExpressionType();

        if (leftOperandType == MathExpressionType_Constant && rightOperandType == MathExpressionType_Constant)
        {
            return MathConstant::From(0);
        }
        else if (leftOperandType == MathExpressionType_Constant)
        {
            return MathMultiplicationExpression::From(MathConstant::From(-1), right->Derivative());
        }
        else if (rightOperandType == MathExpressionType_Constant)
        {
            return left->Derivative();
        }
        else
        {
            return MathSubtractionExpression::From(left->Derivative(), right->Derivative());
        }
    }

    MathExpression* MathSubtractionExpression::Copy()
    {
        return new MathSubtractionExpression(*this);
    }

    MathExpression* MathSubtractionExpression::From(MathExpression* left, MathExpression* right)
    {
        MathExpression* result;

        if (left->GetExpressionType() == MathExpressionType_Constant && right->GetExpressionType() == MathExpressionType_Constant)
        {
            result = MathConstant::From(left->CalculateFor(0) - right->CalculateFor(0));
            delete right;
            delete left;
        }
        else if (left->GetExpressionType() == MathExpressionType_Constant)
        {
            if (left->CalculateFor(0) == 0)
            {
                result = MathMultiplicationExpression::From(MathConstant::From(-1), right);
                delete left;
            }
            else
            {
                return new MathSubtractionExpression(left, right);
            }
        }
        else if (right->GetExpressionType() == MathExpressionType_Constant)
        {
            if (right->CalculateFor(0) == 0)
            {
                delete right;
                return left;
            }
            else
            {
                return new MathSubtractionExpression(left, right);
            }
        }
        else if (left->GetExpressionType() == MathExpressionType_Polynomial && right->GetExpressionType() == MathExpressionType_Polynomial)
        {
            MathPolynomial* leftPolynomial = (MathPolynomial*)left;
            MathPolynomial* rightPolynomial = (MathPolynomial*)right;

            if (leftPolynomial->GetExponent() == rightPolynomial->GetExponent())
            {
                result = MathPolynomial::From(leftPolynomial->GetCoefficient() - rightPolynomial->GetCoefficient(), leftPolynomial->GetExponent());
                delete right;
                delete left;
            }
            else
            {
                return new MathSubtractionExpression(left, right);
            }
        }
        else if (left->GetExpressionType() == MathExpressionType_Variable && right->GetExpressionType() == MathExpressionType_Variable)
        {
            delete right;
            delete left;
            return MathConstant::From(0);
        }
        else return new MathSubtractionExpression(left, right);

        return result;
    }

    /************************************/
    /*** MathMultiplicationExpression ***/
    /************************************/

    MathMultiplicationExpression::MathMultiplicationExpression(MathExpression* left, MathExpression* right) : MathArithmeticExpression(left, right, '*') {}

    MathMultiplicationExpression::MathMultiplicationExpression(const MathMultiplicationExpression& obj) : MathArithmeticExpression(obj) {}

    MathExpression* MathMultiplicationExpression::Derivative()
    {
        MathExpressionType leftOperandType = left->GetExpressionType();

        MathExpressionType rightOperandType = right->GetExpressionType();

        if (leftOperandType == MathExpressionType_Constant && rightOperandType == MathExpressionType_Constant)
        {
            return MathConstant::From(0);
        }
        else if (leftOperandType == MathExpressionType_Constant)
        {
            return MathMultiplicationExpression::From(right->Derivative(), MathConstant::From(left->CalculateFor(0)));
        }
        else if (rightOperandType == MathExpressionType_Constant)
        {
            return MathMultiplicationExpression::From(left->Derivative(), MathConstant::From(right->CalculateFor(0)));
        }
        else
        {
            return MathAdditionExpression::From(
                MathMultiplicationExpression::From(left->Derivative(), right->Copy()),
                MathMultiplicationExpression::From(left->Copy(), right->Derivative())
            );
        }
    }

    MathExpression* MathMultiplicationExpression::Copy()
    {
        return new MathMultiplicationExpression(*this);
    }

    MathExpression* MathMultiplicationExpression::From(MathExpression* left, MathExpression* right)
    {
        MathExpression* result;

        if (left->GetExpressionType() == MathExpressionType_Constant && right->GetExpressionType() == MathExpressionType_Constant)
        {
            result = MathConstant::From(left->CalculateFor(0) * right->CalculateFor(0));
            delete right;
            delete left;
        }
        else if (left->GetExpressionType() == MathExpressionType_Constant)
        {
            if (left->CalculateFor(0) == 0)
            {
                result = MathConstant::From(0);
                delete right;
                delete left;
            }
            else if (left->CalculateFor(0) == 1)
            {
                delete left;
                return right;
            }
            else if (right->GetExpressionType() == MathExpressionType_Variable)
            {
                result = MathPolynomial::From(left->CalculateFor(0), 1);
                delete right;
                delete left;
            }
            else if (right->GetExpressionType() == MathExpressionType_Polynomial)
            {
                MathPolynomial* polynomial = (MathPolynomial*)right;
                result = MathPolynomial::From(left->CalculateFor(0) * polynomial->GetCoefficient(), polynomial->GetExponent());
                delete left;
                delete right;
            }
            else
            {
                return new MathMultiplicationExpression(left, right);
            }
        }
        else if (right->GetExpressionType() == MathExpressionType_Constant)
        {
            if (right->CalculateFor(0) == 0)
            {
                result = MathConstant::From(0);
                delete right;
                delete left;
            }
            else if (right->CalculateFor(0) == 1)
            {
                delete right;
                return left;
            }
            else if (left->GetExpressionType() == MathExpressionType_Variable)
            {
                result = MathPolynomial::From(right->CalculateFor(0), 1);
                delete right;
                delete left;
            }
            else if (left->GetExpressionType() == MathExpressionType_Polynomial)
            {
                MathPolynomial* polynomial = (MathPolynomial*)left;
                result = MathPolynomial::From(right->CalculateFor(0) * polynomial->GetCoefficient(), polynomial->GetExponent());
                delete left;
                delete right;
            }
            else
            {
                return new MathMultiplicationExpression(left, right);
            }
        }
        else if (left->GetExpressionType() == MathExpressionType_Polynomial && right->GetExpressionType() == MathExpressionType_Polynomial)
        {
            MathPolynomial* leftPolynomial = (MathPolynomial*)left;
            MathPolynomial* rightPolynomial = (MathPolynomial*)right;
            result = MathPolynomial::From(leftPolynomial->GetCoefficient() * rightPolynomial->GetCoefficient(), leftPolynomial->GetExponent() + rightPolynomial->GetExponent());
            delete left;
            delete right;
        }
        else if (left->GetExpressionType() == MathExpressionType_Polynomial && right->GetExpressionType() == MathExpressionType_Variable)
        {
            MathPolynomial* leftPolynomial = (MathPolynomial*)left;
            result = MathPolynomial::From(leftPolynomial->GetCoefficient(), leftPolynomial->GetExponent() + 1);
            delete left;
            delete right;
        }
        else if (left->GetExpressionType() == MathExpressionType_Variable && right->GetExpressionType() == MathExpressionType_Polynomial)
        {
            MathPolynomial* rightPolynomial = (MathPolynomial*)right;
            result = MathPolynomial::From(rightPolynomial->GetCoefficient(), rightPolynomial->GetExponent() + 1);
            delete left;
            delete right;
        }
        else if (left->GetExpressionType() == MathExpressionType_Variable && right->GetExpressionType() == MathExpressionType_Variable)
        {
            delete left;
            delete right;
            return MathPolynomial::From(1, 2);
        }
        else return new MathMultiplicationExpression(left, right);

        return result;
    }

    /******************************/
    /*** MathDivisionExpression ***/
    /******************************/

    MathDivisionExpression::MathDivisionExpression(MathExpression* left, MathExpression* right) : MathArithmeticExpression(left, right, '/') {}

    MathDivisionExpression::MathDivisionExpression(const MathDivisionExpression& obj) : MathArithmeticExpression(obj) {}

    MathExpression* MathDivisionExpression::Derivative()
    {
        MathExpressionType leftOperandType = left->GetExpressionType();

        MathExpressionType rightOperandType = right->GetExpressionType();

        if (leftOperandType == MathExpressionType_Constant && rightOperandType == MathExpressionType_Constant)
        {
            return MathConstant::From(0);
        }
        else if (leftOperandType == MathExpressionType_Constant)
        {
            return MathMultiplicationExpression::From(
                MathDivisionExpression::From(
                    MathMultiplicationExpression::From(MathConstant::From(-1), left->Copy()),
                    MathExponationExpression::From(right->Copy(), MathConstant::From(2))
                ),
                right->Derivative()
            );
        }
        else if (rightOperandType == MathExpressionType_Constant)
        {
            return new MathDivisionExpression(left->Derivative(), MathConstant::From(right->CalculateFor(0)));
        }
        else
        {
            return MathDivisionExpression::From(
                MathSubtractionExpression::From(
                    MathMultiplicationExpression::From(left->Derivative(), right->Copy()),
                    MathMultiplicationExpression::From(left->Copy(), right->Derivative())
                ),
                MathExponationExpression::From(right->Copy(), MathConstant::From(2))
            );
        }
    }

    MathExpression* MathDivisionExpression::Copy()
    {
        return new MathDivisionExpression(*this);
    }

    MathExpression* MathDivisionExpression::From(MathExpression* left, MathExpression* right)
    {
        MathExpression* result = NULL;

        if (right->GetExpressionType() == MathExpressionType_Constant && right->CalculateFor(1) == 0)
        {
            throw std::invalid_argument("Division by zero");
        }
        else if (left->GetExpressionType() == MathExpressionType_Constant && right->GetExpressionType() == MathExpressionType_Constant)
        {
            result = MathConstant::From(left->CalculateFor(0) / right->CalculateFor(0));
            delete left;
            delete right;
        }
        else if (left->GetExpressionType() == MathExpressionType_Constant)
        {
            if (left->CalculateFor(0) == 0)
            {
                result = MathConstant::From(0);
                delete left;
                delete right;
            }
            else
            {
                return new MathDivisionExpression(left, right);
            }
        }
        else if (right->GetExpressionType() == MathExpressionType_Constant)
        {
            if (right->CalculateFor(0) == 1)
            {
                delete right;
                return left;
            }
            else
            {
                return new MathDivisionExpression(left, right);
            }
        }
        else
        {
            return new MathDivisionExpression(left, right);
        }

        return result;
    }

    /********************/
    /*** MathFunction ***/
    /********************/

    MathFunction::MathFunction(MathExpression* arg) : MathExpression()
    {
        argument = arg->Copy();
        type = MathExpressionType_Function;
    }

    MathFunction::MathFunction(const MathFunction& obj) : MathExpression()
    {
        argument = obj.argument->Copy();
        type = MathExpressionType_Function;
    }

    MathFunction::~MathFunction()
    {
        #ifdef _MATH_EVALUATOR_DEBUG_VERBOSE
        printf("[[ MathFunction destructor ]]\n");
        #endif
        delete argument;
    }

    /*****************/
    /*** MathSinus ***/
    /*****************/

    MathSinus::MathSinus(MathExpression* arg) : MathFunction(arg) {}

    MathSinus::MathSinus(const MathSinus& obj) : MathFunction(obj) {}

    MathExpression* MathSinus::Derivative()
    {
        return MathMultiplicationExpression::From(MathCosinus::From(argument->Copy()), argument->Derivative());
    }

    MathExpression* MathSinus::Copy()
    {
        return new MathSinus(*this);
    }

    double MathSinus::CalculateFor(double x)
    {
        return sin(argument->CalculateFor(x));
    }

    void MathSinus::Print()
    {
        printf("sin(");
        argument->Print();
        printf(")");
    }


    MathExpression* MathSinus::From(MathExpression* arg)
    {
        if (arg->GetExpressionType() == MathExpressionType_Constant)
        {
            MathExpression* result = MathConstant::From(sin(arg->CalculateFor(0)));
            delete arg;
            return result;
        }
        else
        {
            return new MathSinus(arg);
        }
    }

    /*******************/
    /*** MathCosinus ***/
    /*******************/

    MathCosinus::MathCosinus(MathExpression* arg) : MathFunction(arg) {}

    MathCosinus::MathCosinus(const MathCosinus& obj) : MathFunction(obj) {}

    MathExpression* MathCosinus::Derivative()
    {
        return MathMultiplicationExpression::From(MathConstant::From(-1), MathMultiplicationExpression::From(MathSinus::From(argument->Copy()), argument->Derivative()));
    }

    MathExpression* MathCosinus::Copy()
    {
        return new MathCosinus(*this);
    }

    double MathCosinus::CalculateFor(double x)
    {
        return cos(argument->CalculateFor(x));
    }

    void MathCosinus::Print()
    {
        printf("cos(");
        argument->Print();
        printf(")");
    }

    MathExpression* MathCosinus::From(MathExpression* arg)
    {
        if (arg->GetExpressionType() == MathExpressionType_Constant)
        {
            MathExpression* result = MathConstant::From(cos(arg->CalculateFor(0)));
            delete arg;
            return result;
        }
        else
        {
            return new MathCosinus(arg);
        }
    }

    /*******************/
    /*** MathTangent ***/
    /*******************/

    MathTangent::MathTangent(MathExpression* arg) : MathFunction(arg) {}

    MathTangent::MathTangent(const MathTangent& obj) : MathFunction(obj) {}

    MathExpression* MathTangent::Derivative()
    {
        return MathDivisionExpression::From(argument->Derivative(), MathExponationExpression::From(MathCosinus::From(argument->Copy()), MathConstant::From(2)));
    }

    MathExpression* MathTangent::Copy()
    {
        return new MathTangent(*this);
    }

    double MathTangent::CalculateFor(double x)
    {
        return tan(argument->CalculateFor(x));
    }

    void MathTangent::Print()
    {
        printf("tan(");
        argument->Print();
        printf(")");
    }

    MathExpression* MathTangent::From(MathExpression* arg)
    {
        if (arg->GetExpressionType() == MathExpressionType_Constant)
        {
            MathExpression* result = MathConstant::From(tan(arg->CalculateFor(0)));
            delete arg;
            return result;
        }
        else
        {
            return new MathTangent(arg);
        }
    }

    /*********************/
    /*** MathCotangent ***/
    /*********************/

    MathCotangent::MathCotangent(MathExpression* arg) : MathFunction(arg) {}

    MathCotangent::MathCotangent(const MathCotangent& obj) : MathFunction(obj) {}

    MathExpression* MathCotangent::Derivative()
    {
        return MathMultiplicationExpression::From(MathConstant::From(-1), MathDivisionExpression::From(argument->Derivative(), MathExponationExpression::From(MathSinus::From(argument->Copy()), MathConstant::From(2))));
    }

    MathExpression* MathCotangent::Copy()
    {
        return new MathCotangent(*this);
    }

    double MathCotangent::CalculateFor(double x)
    {
        return 1 / tan(argument->CalculateFor(x));
    }

    void MathCotangent::Print()
    {
        printf("cot(");
        argument->Print();
        printf(")");
    }

    MathExpression* MathCotangent::From(MathExpression* arg)
    {
        if (arg->GetExpressionType() == MathExpressionType_Constant)
        {
            MathExpression* result = MathConstant::From(1 / tan(arg->CalculateFor(0)));
            delete arg;
            return result;
        }
        else
        {
            return new MathCotangent(arg);
        }
    }

    /***********************/
    /*** MathLogarithm10 ***/
    /***********************/

    MathLogarithm10::MathLogarithm10(MathExpression* arg) : MathFunction(arg) {}

    MathLogarithm10::MathLogarithm10(const MathLogarithm10& obj) : MathFunction(obj) {}

    MathExpression* MathLogarithm10::Derivative()
    {
        return MathDivisionExpression::From(argument->Derivative(), MathMultiplicationExpression::From(argument->Copy(), MathLogarithmNatural::From(MathConstant::From(10))));
    }

    MathExpression* MathLogarithm10::Copy()
    {
        return new MathLogarithm10(*this);
    }

    double MathLogarithm10::CalculateFor(double x)
    {
        return log10(argument->CalculateFor(x));
    }

    void MathLogarithm10::Print()
    {
        printf("log(");
        argument->Print();
        printf(")");
    }

    MathExpression* MathLogarithm10::From(MathExpression* arg)
    {
        if (arg->GetExpressionType() == MathExpressionType_Constant)
        {
            MathExpression* result = MathConstant::From(log10(arg->CalculateFor(0)));
            delete arg;
            return result;
        }
        else
        {
            return new MathLogarithm10(arg);
        }
    }

    /****************************/
    /*** MathLogarithmNatural ***/
    /****************************/

    MathLogarithmNatural::MathLogarithmNatural(MathExpression* arg) : MathFunction(arg) {}

    MathLogarithmNatural::MathLogarithmNatural(const MathLogarithmNatural& obj) : MathFunction(obj) {}

    MathExpression* MathLogarithmNatural::Derivative()
    {
        return MathDivisionExpression::From(argument->Derivative(), argument->Copy());
    }

    MathExpression* MathLogarithmNatural::Copy()
    {
        return new MathLogarithmNatural(*this);
    }

    double MathLogarithmNatural::CalculateFor(double x)
    {
        return log(argument->CalculateFor(x));
    }

    void MathLogarithmNatural::Print()
    {
        printf("ln(");
        argument->Print();
        printf(")");
    }

    MathExpression* MathLogarithmNatural::From(MathExpression* arg)
    {
        if (arg->GetExpressionType() == MathExpressionType_Constant)
        {
            MathExpression* result = MathConstant::From(log(arg->CalculateFor(0)));
            delete arg;
            return result;
        }
        else
        {
            return new MathLogarithmNatural(arg);
        }
    }

}
