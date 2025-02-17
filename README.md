# Ewaluator wyrażeń matematycznych

## Autor

Artur Kręgiel, wtorek parzysty godz. 15:15

## Opis

Program wykorzystuje technikę [*Recursive descent parsing*](https://www.youtube.com/watch?v=SToUyjAsaFk&t=854s) w celu przeanalizowanego ciągu znaków i zbudowania *abstrakcyjnego drzewa składniowego* [(ang. *abstract syntax tree*, AST)](https://en.wikipedia.org/wiki/Abstract_syntax_tree), którego węzłami są obiekty klas reprezentujących różne wyrażenia matematyczne (zmienną *x*, stałe (liczby, stała *e*), dodawanie, odejmowanie, dzielenie etc.).

Mając gotowe drzewo AST można obliczyć wartość danego wyrażenia dla zadanego *x* oraz wyznaczyć pochodną tego wyrażenia (reprezentowaną jako osobne drzewo AST wyrażenia matematycznego)

Obsługiwane są funkcje matematyczne:

- `sin(...)` - funkcja *sinus*
- `cos(...)` - funkcja *cosinus*
- `tan(...)` - *tangens*
- `cot(...)` - *cotangens*
- `ln(...)` - logarytm naturalny
- `log(...)` - logarytm o podstawie 10

Przykładowe wyrażenia:

```
x^2.5+3*x+5
```

```
-e^(x^3+2)+4*-(x^5+78.89-67*x)-sin(x^2+4/x-3)
```

```
(x^2.5-4*x)/(3*x+5)
```

Projekt realizowany w ramach zaliczenia eksternistycznego zajęć laboratoryjnych z Podstaw Programowania u [dr inż. Pawła Rogalińskiego]((https://wit.pwr.edu.pl/wydzial/struktura-organizacyjna/pracownicy/pawel-rogalinski)).