#include "parser.h"
#include "QDebug"
#include "QFile"
Parser::Parser()
{

}
QString Parser::getStringFile(QString directory)
{
    QFile file(directory);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
    }
    QString input;
    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull()) {
        input += line ;
        line = in.readLine();
        input += "\n";
    }
    return (input);
}
int Parser::_3bas(QString s2)
{
    //s2 = getStringFile("/home/mogz/Desktop/Tiny-Compiler/parser_input.txt");//media/sf_C_DRIVE/Users/moham/QT/Tiny-Compiler/parser_input.txt
    //s2 = input_preprocessing(s2);
    //qDebug() << s2;
    tokens = s2.split(QRegExp("\n"));
    for(int i=0;i<tokens.length();i++)
        tokens[i].remove(" ");
    token = tokens[token_index].toStdString();
    token_index++;
    program();
    process_edges();
    //file.close();
    return error;
}
stmtTypes Parser::getStmtType(string s)
{
    s = s.substr(s.find(",") + 1, s.length() - s.find(",")+1);
    qDebug() << QString::fromStdString( s)<<"\n";
    if (s == "IF")
        return IF;
    if (s == "IDENTIFIER")
        return ASSIGN;
    if (s == "READ")
        return READ;
    if (s == "WRITE")
        return WRITE;
    if (s == "REPEAT")
        return REPEAT;
    else
    {

        cout << tokens[token_index].toStdString()<<"\n";
        return ERROR;
    }
}
void  Parser::match(string s)
{
    if (s == token)
    {
        token =  tokens[token_index].toStdString();
        token_index++;
    }
    else
    {
        error=2;
        cout << endl << "-- Program not found" << endl;
        //exit(1);
    }
}
// program -> stmt-seq
void  Parser::program()
{
    stmt_seq();
    cout << endl << "-- Program found" << endl;
}
// stmt-seq -> stmt {; stmt}
void  Parser::stmt_seq()
{
    stmt();
    while (token == ";,SEMICOLON")
    {
        match(";,SEMICOLON");
        stmt();
    }
}
// stmt -> if-stmt | repeat-stmt | assign-stmt | read-stmt | write-stmt
void  Parser::stmt()
{
    cout << "1-" << token <<"\n" ;
    switch (getStmtType(token))
    {
    case READ:
        read_stmt();
        break;
    case WRITE:
        write_stmt();
        break;
    case IF:
        if_stmt();
        break;
    case REPEAT:
        repeat_stmt();
        break;
    case ASSIGN:
        cout << "2-" << token <<"\n" ;
        assign_stmt();
        break;
    case ERROR:
        error =1;
        cout << "no stmt found\n";
        //exit(1);
        break;
    default:
        break;
    }
}

// if-stmt -> if exp then stmt-seq [else stmt-seq] end
void  Parser::if_stmt()
{
    match("if,IF");
    pre_update_edge();
    Nodes.append({true,currentx,currenty,"if"});
    post_update_edge(false);

    currenty+=100;//child y
    exp();
    currenty-=100;//parent y
    match("then,THEN");

    currentx += 150;//child2 x
    currenty += 100;//child y
    stmt_seq();
    currenty -= 100;//parent y
    if (token == "else,ELSE")
        stmt_seq();
    match("end,END");
    currentx +=150;//friend x;
}

// repeat-stmt -> repeat stmt-seq until exp
void  Parser::repeat_stmt()
{
    match("repeat,REPEAT");
    pre_update_edge();
    Nodes.append({true,currentx,currenty,"repeat"});
    post_update_edge(false);

    currenty+=100;//child y
    stmt_seq();
    currenty-=100;//parent y
    match("until,UNTIL");

    currenty +=100 ;//child y
    exp();
    currenty -= 100;//parent y
    cout << "- repeat found" << endl;
    currentx +=150; //friend x
}

// assign-stmt -> identifier := exp
void  Parser::assign_stmt()
{

    pre_update_edge();
    Nodes.append({true,currentx,currenty,"assign\n   "+token.substr(0, token.find(","))});
    post_update_edge(false);
    match(token.substr(0, token.find(",")) + ",IDENTIFIER");
    cout << "3-" << token <<"\n" ;
    match(":=,ASSIGN");
    cout << "4-" << token <<"\n" ;
    currenty +=100;//child y
    exp();
    cout << "-" << token <<"\n" ;
    currenty -=100;//parent y
    currentx +=150;//friend x;
}

// read-stmt -> read identifier
void  Parser::read_stmt()
{
    pre_update_edge();
    Nodes.append({true,currentx,currenty,"read\n"});
    post_update_edge(true);
    match("read,READ");
    token.erase(0, token.find(",") + 1);
    match("IDENTIFIER");
    currentx += 150;//friend x
}

// write-stmt -> write exp
void  Parser::write_stmt()
{
    pre_update_edge();
    Nodes.append({true, currentx, currenty, "write"});
    post_update_edge(false);
    match("write,WRITE");
    currenty +=100;//child y
    exp();
    currenty -=100;//parent y
    currentx +=150;//friend x;
}
// exp -> simple-exp [comparison-op simple-exp]
void  Parser::exp()
{
    cout << "6-" << token <<"\n" ;
    if(tokens[token_index] == "<,LESSTHAN" || tokens[token_index] == "=,EQUAL")
       currenty+=100;
    simple_exp();
    if (token == "<,LESSTHAN" || token == "=,EQUAL")
    {
        currenty -= 100; //parenty
        comparison_op();
        currenty +=100; //child y
        currentx += 150; //child x
        simple_exp();
        currenty -=100;//parent y
    }
}

// comparison-op -> < | =
void  Parser::comparison_op()
{
    if (token == "<,LESSTHAN")
    {
        pre_update_edge();
        Nodes.append({false, currentx, currenty, "op\n(<)"});
        post_update_edge(false);
        match("<,LESSTHAN");
    }
    if (token == "=,EQUAL")
    {
        pre_update_edge();
        Nodes.append({false,currentx,currenty,"op\n(=)"});
        post_update_edge(false);
        match("=,EQUAL");
    }
}

// simple-exp -> term { addop term }
void  Parser::simple_exp()
{
    if(tokens[token_index] == "+,PLUS"  || tokens[token_index] =="-,MINUS")
       currenty+=100;
    cout << "7-" << token <<"\n" ;
    term();
    while (token == "+,PLUS" || token == "-,MINUS")
    {
        cout << "8-" << token <<"\n" ;
        currenty -= 100; //parenty
        addop();
        cout << "10-" << token <<"\n" ;
        currenty +=100; //child y
        currentx += 150; //child x
        cout << "9-" << token <<"\n" ;
        term();

        currenty -=100;//parent y
    }
}
// addop -> + | -
void  Parser::addop()
{
    if (token == "+,PLUS")
    {
        pre_update_edge();
        Nodes.append({false,currentx,currenty,"op\n(+)"});
        post_update_edge(false);
        match("+,PLUS");
    }
    if (token == "-,MINUS")
    {
        pre_update_edge();
        Nodes.append({false,currentx,currenty,"op\n(-)"});
        post_update_edge(false);
        match("-,MINUS");
    }
}

// term -> factor { mulop factor }
void  Parser::term()
{
    if(tokens[token_index] == "*,MULT"  || tokens[token_index] == "/,DIV")
       currenty+=100;
    factor(currentx,currenty-100);
    while (token == "*,MULT" || token == "/,DIV")
    {

        currenty -= 100; //parenty
         mulop();
        currenty +=100; //child y
        currentx += 150; //child x
        factor(currentx-150,currenty-100);
        currenty -=100;//parent y
    }

}
// mulop -> * | /
void  Parser::mulop()
{
    if (token == "*,MULT")
    {
        pre_update_edge();
        Nodes.append({false,currentx,currenty,"op\n(*)"});
        post_update_edge(false);
        match("*,MULT");
        cout << " * -> ";
    }
    if (token == "/,DIV")
    {
        pre_update_edge();
        Nodes.append({false,currentx,currenty,"op\n(/)"});
        post_update_edge(false);
        match("/,DIV");
    }
}

// factor -> (exp) | number | identifier
void Parser:: factor(int x,int y )
{

    if (token == "(,OPENBRACKET")
    {
        cout << "12- " << token << "\n" ;
        match("(,OPENBRACKET");
        exp();
        match("),CLOSEDBRACKET");
    }
//    else if(token.substr(token.find(",") + 1, token.length() -1) == "NUMBER"
//            && ( tokens[token_index] == "+,PLUS" || tokens[token_index] == "-,MINUS" || tokens[token_index] == "*,MULT" || tokens[token_index] == "/,DIV")
//            && tokens[token_index-2 ]!= ":=,ASSIGN")

//    {
//        match("+,PLUS");
//        cout <<">>>" <<tokens[token_index-2].toStdString() << ">> "<<token<<" >>"<<tokens[token_index].toStdString()<<"\n";
//    }
    else if (token.substr(token.find(",") + 1, token.length() -1) == "NUMBER")
    {
        cout << ">>>>>>>>>>>>>\n";
        pre_update_edge();
        Nodes.append({false,currentx,currenty,"NUMBER\n"+token.substr(0, token.find(","))});
        match(token.substr(0, token.find(",")) + ",NUMBER");

    }
    else if (token.substr(token.find(",") + 1, token.length() - token.find(",") + 1) == "IDENTIFIER")
    {
        pre_update_edge();
        Nodes.append({false,currentx,currenty,"IDENTIFIER\n"+token.substr(0, token.find(","))});
        match(token.substr(0, token.find(",")) + ",IDENTIFIER");
    }
}


QString Parser::input_preprocessing(QString s2){

    return s2;
}
void Parser::process_edges(){
    int len = Nodes.length();
    for(int i = 0; i < len; i++){
        bool flag = false;
        int minIndex = 0;
        for(int j = 0; j < len; j++){
            if (j != i){
                if(Nodes[i].Rect && Nodes[j].Rect && Nodes[i].y == Nodes[j].y && Nodes[i].x > Nodes[j].x){
                    if(!flag){
                        minIndex = j;
                        flag = true;
                    }
                    if((Nodes[i].x - Nodes[j].x) < (Nodes[i].x - Nodes[minIndex].x))
                        minIndex = j;
                }
                if(!Nodes[i].Rect && !Nodes[j].Rect && Nodes[i].x == Nodes[j].x && Nodes[i].y - Nodes[j].y == 100)
                    Edges.append({false, Nodes[j].x, Nodes[j].y, Nodes[i].x, Nodes[i].y});
                if(Nodes[i].text == "if" && Nodes[j].x - Nodes[i].x == 300 && Nodes[j].y - Nodes[i].y == 100)
                    Edges.append({false, Nodes[i].x, Nodes[i].y, Nodes[j].x, Nodes[j].y});
                if(Nodes[i].text == "repeat" && Nodes[j].y - Nodes[i].y == 100
                        && Nodes[j].x > Nodes[i].x && !Nodes[j].Rect && Nodes[j].text.substr(0, 2) == "op"){
                    Edges.append({false, Nodes[i].x, Nodes[i].y, Nodes[j].x, Nodes[j].y});
                }
            }
        }
        if(flag)
            Edges.append({true, Nodes[minIndex].x, Nodes[minIndex].y, Nodes[i].x, Nodes[i].y});
    }
     for(int i = 0; i < Edges.length(); i++){
         if(Edges[i].y2 -Edges[i].y1 >100)
             Edges[i].y2 -=100;
     }
}

void Parser::pre_update_edge(){
    if (currentEdge != NULL){
        currentEdge->x2 = currentx;
        currentEdge->y2 = currenty;
        Edges.append(*currentEdge);
        currentEdge = NULL;
    }
}

void Parser::post_update_edge(bool isFriend){
    currentEdge = new Edge;
    currentEdge->isFriend = isFriend;
    currentEdge->x1 = currentx;
    currentEdge->y1 = currenty;
}

QVector<Node> Parser::get_nodes()
{
    return Nodes;
}
QVector<Edge> Parser::get_edges()
{
    return Edges;
}

void Parser::setTokenIndex(int x)
{
    token_index = x;
    currentx = 0;
    currenty = 0;
    error = 0;
}

void Parser::clear_parser()
{
    Edges.clear();
    Nodes.clear();
}
