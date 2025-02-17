#pragma once

/*
 * Autor: Artur Kregiel
 * Grupa: WTP 15:15    (wtorek parzysty godz 15:15)
 * Temat: Ewaluator wyrazen matematycznych
 */

// Definicje i prototypy

//#define _MATH_EVALUATOR_DEBUG_VERBOSE

namespace MathFormulaEvaluator
{
    enum MathExpressionType
    {
        MathExpressionType_Variable,
        MathExpressionType_Polynomial,
        MathExpressionType_Constant,
        MathExpressionType_EulerNumber,
        MathExpressionType_Arithmetic,
        MathExpressionType_Exponation,
        MathExpressionType_Function
    };

	// klasa bazowa reprezentuj�ca wyra�enie matematyczne
    class MathExpression
    {
        protected:

        MathExpressionType type;

        static unsigned int objectCount;

        MathExpression()
        {
            objectCount++;
        }

        public:

        virtual ~MathExpression()
        {
            #ifdef _MATH_EVALUATOR_DEBUG_VERBOSE
            printf("[[ MathExpression destructor ]]\n");
            printf("((( ObjectCount: %d )))\n", GetObjectCount());
            #endif

            objectCount--;
        }

        static unsigned int GetObjectCount()
        {
            return objectCount;
        }

        // zwraca priorytet operatora (0 - je�li nie jest operatorem)
        static unsigned int GetOperatorPriority(char op)
        {
            switch (op)
            {
                case '+':
                case '-':
                    return 1;
                case '*':
                case '/':
                    return 2;
                case '^':
                    return 3;
                default:
                    return 0;
            }
        }

        MathExpressionType GetExpressionType()
        {
            return type;
        }

        // oblicza warto�� wyra�enia dla podanej warto�ci zmiennej
        virtual double CalculateFor(double x) = 0;

        // wypisuje wyra�enie w postaci napisu
        virtual void Print() = 0;

        // zwraca kopi� wyra�enia
        virtual MathExpression* Copy() = 0;

        // zwraca pochodn� wyra�enia w postaci napisu
        virtual MathExpression* Derivative() = 0;
    };

	// zmienna x
    class MathVariable : public MathExpression
    {
        public:

        MathVariable() : MathExpression()
        {
            type = MathExpressionType_Variable;
        }

        // Konstruktor kopiuj�cy
        MathVariable(const MathVariable& obj) : MathExpression()
        {
            type = obj.type;
        }

        virtual ~MathVariable()
        {
            #ifdef _MATH_EVALUATOR_DEBUG_VERBOSE
            printf("[[ MathVariable destructor ]]\n");
            #endif
        }

        double CalculateFor(double x)
        {
            return x;
        }

        void Print()
        {
            printf("x");
        }

        MathExpression* Copy()
        {
            return new MathVariable(*this);
        }

        MathExpression* Derivative();
    };

    // wielomian
    class MathPolynomial: public MathExpression
    {
        protected:

        double coefficient, exponent;

        MathPolynomial(double coefficient, double exponent);

        // Konstruktor kopiuj�cy
        MathPolynomial(const MathPolynomial& obj);

        public:

        static MathExpression* From(double coefficient, double exponent);

        virtual ~MathPolynomial();

        double CalculateFor(double x);

        void Print();

        double GetCoefficient()
        {
            return coefficient;
        }

        double GetExponent()
        {
            return exponent;
        }

        MathExpression* Copy();

        MathExpression* Derivative();
    };

	// sta�a (liczba)
    class MathConstant : public MathExpression
    {
        private:

        double value;

        protected:

        MathConstant(double val);

        // Konstruktor kopiuj�cy
        MathConstant(const MathConstant& obj);

        public:

        static MathExpression* From(double val)
        {
            return new MathConstant(val);
        }

        virtual ~MathConstant()
        {
            #ifdef _MATH_EVALUATOR_DEBUG_VERBOSE
            printf("[[ MathConstant destructor ]]\n");
            #endif
        }

        double CalculateFor(double x)
        {
            return value;
        }

        void Print();

        MathExpression* Copy()
        {
            return new MathConstant(*this);
        }

        MathExpression* Derivative()
        {
            return MathConstant::From(0);
        }
    };

	// liczba e
    class MathEulerNumber : public MathConstant
    {
        public:

        MathEulerNumber();

        // Konstruktor kopiuj�cy
        MathEulerNumber(const MathEulerNumber& obj);

        virtual ~MathEulerNumber();

        void Print();

        MathExpression* Copy();
    };

	// wyra�enie arytmetyczne
    class MathArithmeticExpression : public MathExpression
    {
        protected:

        MathExpression* left;
        MathExpression* right;
        char op;

        MathArithmeticExpression(MathExpression* left, MathExpression* right, char op);

        // Konstruktor kopiuj�cy
        MathArithmeticExpression(const MathArithmeticExpression& obj);

        public:

        virtual ~MathArithmeticExpression();

        double CalculateFor(double x);

        char GetOperator();	// zwraca operator arytmetyczny (+-*/^)

        void Print();

        virtual MathExpression* Copy() = 0;

        virtual MathExpression* Derivative() = 0;
    };

	// pot�gowanie
    class MathExponationExpression : public MathArithmeticExpression
    {
        protected:

        MathExponationExpression(MathExpression* left, MathExpression* right);

        // Konstruktor kopiuj�cy
        MathExponationExpression(const MathExponationExpression& obj);


        public:
        //~MathExponationExpression();

        MathExpression* Derivative();

        MathExpression* Copy();

        static MathExpression* From(MathExpression* left, MathExpression* right);
    };

	// dodawanie
    class MathAdditionExpression : public MathArithmeticExpression
    {
        protected:

        MathAdditionExpression(MathExpression* left, MathExpression* right);

        // Konstruktor kopiuj�cy
        MathAdditionExpression(const MathAdditionExpression& obj);

        public:

        MathExpression* Derivative();

        MathExpression* Copy();

        static MathExpression* From(MathExpression* left, MathExpression* right);
    };

	// odejmowanie
    class MathSubtractionExpression : public MathArithmeticExpression
    {
        protected:

        MathSubtractionExpression(MathExpression* left, MathExpression* right);

        // Konstruktor kopiuj�cy
        MathSubtractionExpression(const MathSubtractionExpression& obj);

        public:

        //~MathSubtractionExpression();

        MathSubtractionExpression operator = (const MathSubtractionExpression& obj);

        MathExpression* Derivative();

        MathExpression* Copy();

        static MathExpression* From(MathExpression* left, MathExpression* right);
    };

	// mno�enie
    class MathMultiplicationExpression : public MathArithmeticExpression
    {
        protected:

        MathMultiplicationExpression(MathExpression* left, MathExpression* right);

        // Konstruktor kopiuj�cy
        MathMultiplicationExpression(const MathMultiplicationExpression& obj);

        public:

        MathExpression* Derivative();

        MathExpression* Copy();

        static MathExpression* From(MathExpression* left, MathExpression* right);
    };

	// dzielenie
    class MathDivisionExpression : public MathArithmeticExpression
    {
        protected:

        MathDivisionExpression(MathExpression* left, MathExpression* right);

        // Konstruktor kopiuj�cy
        MathDivisionExpression(const MathDivisionExpression& obj);

        public:

        MathExpression* Derivative();

        MathExpression* Copy();

        static MathExpression* From(MathExpression* left, MathExpression* right);
    };

	// funkcja
    class MathFunction : public MathExpression
    {
        protected:

        MathExpression* argument;

        MathFunction(MathExpression* arg);

        MathFunction(const MathFunction& obj);

        public:

        virtual ~MathFunction();

        virtual double CalculateFor(double x) = 0;

        virtual void Print() = 0;

        virtual MathExpression* Derivative() = 0;

        virtual MathExpression* Copy() = 0;
    };

	// sinus
    class MathSinus : public MathFunction
    {
        protected:

        MathSinus(MathExpression* arg);

        MathSinus(const MathSinus& obj);

        public:

        double CalculateFor(double x);

        void Print();

        MathExpression* Copy();

        MathExpression* Derivative();

        static MathExpression* From(MathExpression* arg);

    };

	// cosinus
    class MathCosinus : public MathFunction
    {
        protected: 

        MathCosinus(MathExpression* arg);

        MathCosinus(const MathCosinus& obj);

        public:

        double CalculateFor(double x);

        void Print();

        MathExpression* Copy();

        MathExpression* Derivative();

        static MathExpression* From(MathExpression* arg);
    };

	// tangens
    class MathTangent : public MathFunction
    {
        protected:

        MathTangent(MathExpression* arg);

        MathTangent(const MathTangent& obj);

        public:

        double CalculateFor(double x);

        void Print();

        MathExpression* Copy();

        MathExpression* Derivative();

        static MathExpression* From(MathExpression* arg);
    };

	// cotangens
    class MathCotangent : public MathFunction
    {
        protected:

        MathCotangent(MathExpression* arg);

        MathCotangent(const MathCotangent& obj);

        public:

        double CalculateFor(double x);

        void Print();

        MathExpression* Copy();

        MathExpression* Derivative();

        static MathExpression* From(MathExpression* arg);
    };

	// logarytm przy podstawie 10
    class MathLogarithm10 : public MathFunction
    {
        protected:

        MathLogarithm10(MathExpression* arg);

        MathLogarithm10(const MathLogarithm10& obj);

        public:

        double CalculateFor(double x);

        void Print();

        MathExpression* Copy();

        MathExpression* Derivative();

        static MathExpression* From(MathExpression* arg);
    };

	// logarytm naturalny
    class MathLogarithmNatural : public MathFunction
    {
        protected:

        MathLogarithmNatural(MathExpression* arg);

        MathLogarithmNatural(const MathLogarithmNatural& obj);

        public:

        double CalculateFor(double x);

        void Print();

        MathExpression* Copy();

        MathExpression* Derivative();

        static MathExpression* From(MathExpression* arg);
    };
}
