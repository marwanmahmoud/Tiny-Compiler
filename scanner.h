#ifndef SCANNER_H
#define SCANNER_H
#include <iostream>
#include <string>
#include <fstream>
#include <QString>
#include <QDebug>
#include <QFile>
using namespace std;
enum DFAstates {
    START,
    INCOMMENT,
    INNUM,
    INID,
    INASSIGN,
    DONE
};
class Scanner
{
private:
    //set START to be intial state
    DFAstates state = START;
    string RES_WORDS[8] = { "if","then","else","end","repeat","until","read","write" };
public:
    Scanner();
    QString getToken(string);
    QString getStringFile(QString directory);
    bool isDigit(char);
    bool isLetter(char);
    bool isSpace(char);
    bool isSymbol(char);
};
#endif // SCANNER_H
