#include "scanner.h"
Scanner::Scanner()
{
}
QString Scanner::getToken(string l)
{
    QString result = "";
    string mytoken;
    if(l.length() == 0)
        return "";
    bool res_flag = 0;
    int i = 0;
    while (state != DONE)
    {
        switch (state)
        {
        case START:
            if (isSpace(l[i]))
            {
                i++;
                if (i == l.length())
                    state = DONE;
                else
                    state = START;
            }
            else if (isDigit(l[i]))
            {
                state = INNUM;
            }
            else if (isLetter(l[i]))
            {
                state = INID;
            }
            else if (l[i] == ':')
            {
                state = INASSIGN;
            }
            else if (l[i] == '{')
            {
                i++;
                state = INCOMMENT;
            }
            else if (isSymbol(l[i]))
            {
                switch (l[i])
                {
                case ';':
                    break;
                default:
                    qDebug() << l[i] << " , Symbol \n ";
                    break;
                }
                i++;
                if (i == l.length())
                    state = DONE;
                else
                    state = START;
            }
            else
                state = DONE;
            break;

        case INCOMMENT:
            if (state == INCOMMENT)
            {
                while (l[i] != '}')
                {
                    i++;
                }
                i++;
                if (i == l.length())
                    state = DONE;
                else
                    state = START;
            }
            break;

        case INNUM:
            while (isDigit(l[i]))
            {
                mytoken += l[i];
                i++;
            }
            result += QString::fromStdString(mytoken) + " ,number\n";

            mytoken = "";
            if (i == l.length())
                state = DONE;
            else
                state = START;
            break;

        case INID:
            while (isLetter(l[i]))
            {
                mytoken += l[i];
                i++;
            }
            for (int i = 0; i < 8; i++)
            {
                if (RES_WORDS[i] == mytoken)
                    res_flag = 1;
            }
            if (res_flag)
            {
                result += QString::fromStdString(mytoken) + " ,Reserved word\n";

            }
            else
            {
                result +=  QString::fromStdString(mytoken) + " ,identifier\n";

            }
            mytoken = "";
            res_flag = 0;
            if (i == l.length())
                state = DONE;
            else
                state = START;
            break;

        case INASSIGN:
            if (l[i] == ':')
            {
                i += 2;
                result +=  ":= , assign\n";

                state = START;
            }
            else
            {
                if (i == l.length())
                    state = DONE;
                else
                    state = START;
            }
            break;
        case DONE:
            break;
        }
    }
    state = START;
    return result;
}

bool Scanner::isDigit(char d) { return (d >= '0' && d <= '9'); }

bool Scanner::isLetter(char l) { return (l >= 'a' && l <= 'z' || l >= 'A' && l <= 'Z'); }

bool Scanner::isSpace(char s) { return (s == ' ' || s == '\t' || s == '\n'); }

bool Scanner::isSymbol(char c) { return (c == '+' || '-' || '*' || '/' || '=' || '<' || '(' || ')' || ';'); }
