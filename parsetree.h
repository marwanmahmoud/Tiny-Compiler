#ifndef PARSETREE_H
#define PARSETREE_H

#include <QWidget>
#include "QGraphicsScene"
#include "QGraphicsRectItem"
#include "QGraphicsEllipseItem"
#include "QGraphicsLineItem"

class ParseTree : public QWidget
{
    Q_OBJECT
private:
    QGraphicsScene* scene;
public:
    explicit ParseTree(QWidget *parent = nullptr);
    QGraphicsRectItem* addRectangleNode(int x,int y,QString text);
    QGraphicsEllipseItem* addEllipseNode(int x,int y,QString text);
    QGraphicsLineItem *addEdgeChild(int x1,int y1,int x2,int y2);
    QGraphicsLineItem *addEdgeFriend(int x1,int y1,int x2,int y2);


signals:

};

#endif // PARSETREE_H
