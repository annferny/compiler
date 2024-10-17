/*
Programmieren Sie verschiedene Varianten eines Interpreters zur Berechnung von
arithmetischen Ausdrücken (Grundrechenarten). Implementieren Sie diese Interpreter
nach dem Verfahren des rekursiven Abstiegs. Programmieren Sie dazu folgende
Funktionen:
- lex, ein einfacher Lexer, der Zahlen und Sonderzeichen (Operatoren und Klammern) erkennt.
- expr
- term
- faktor
- main zum Test des Ganzen
Verwenden Sie die in der Vorlesung vorgestellten Grammatiken
*/

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

typedef struct morph
{
    int mc;
    union
    {
        double dval;
        char cval;
    };
} MORPHEM;

enum mcodes
{
    mempty,
    mop,
    mdbl
};

static MORPHEM m;

double expr(void);
double term(void);
double fac();

void lex(char *pX)
{
    static char *px;
    /* Initialisierung*/
    if (pX != NULL)
    {
        m.mc = mempty;
        px = pX;
    }
    /* lexiaklische Analyse */
    if (*px == '\0')
    {
        m.mc = mempty;
    }
    else
    {
        for (; *px == ' ' || *px == '\t'; px++)
            ;
        if (isdigit(*px) || *px == '.')
        // numeral
        {
            m.dval = strtod(px, &px);
            m.mc = mdbl;
        }
        else
            // Symbol
            switch (*px)
            {
            case '+':
            case '*':
            case '-':
            case '/':
            case '(':
            case ')':
                m.cval = *px++;
                m.mc = mop;
                break;
            // end or wrong char
            default:
                m.mc = mempty;
            }
    }
}

double expr(void)
{
    double tmp = term();
    if (m.mc == mop && (m.cval == '+' || m.cval == '-'))
    {
        if (m.cval == '+')
        {
            lex(NULL);
            tmp += expr();
        }
        if(m.cval == '-')
        {
            lex(NULL);
            tmp -= expr();
        }
    }
    return tmp;
}

double term(void)
{
    double tmp = fac();
    if (m.mc == mop && (m.cval == '*' || m.cval == '/'))
    {
        if (m.cval == '*')
        {
            lex(NULL);
            tmp *= term();
        }
        if (m.cval == '/')
        {
            lex(NULL);
            tmp /= term();
        }
    }
    return tmp;
}

double fac()
{
    double tmp;
    if (m.mc == mop)
    {
        if (m.cval == '(')
        {
            lex(NULL);
            tmp = expr();
            if (m.mc != mop || m.cval != ')')
                exit(1);
            lex(NULL);
        }
        else
            exit(1);
    }
    else if (m.mc == mdbl)
    {
        tmp = m.dval;
        lex(NULL);
    }
    else
        exit(1);
    return tmp;
}

// 5+2*3
int main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("Error: input expression\n");
        exit(-1);
    }
    char *pBuf = argv[1];
    printf("%s\n", pBuf);
    lex(pBuf);
    printf("%-10.4lf\n", expr());

    return 0;
}