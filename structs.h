#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED
#include <stdlib.h>
#include <iterator>

struct Point{
    double x,y;
    Point(double _x, double _y): x(_x),y(_y){}
    Point(): x(0),y(0){}
    void setX(double _x){x=_x;}
    void setY(double  _y){y=_y;}
    double getX(){return x;}
    double getY(){return y;}

    void setToPoint(Point p){ setX(p.x);setY(p.y);}
    void print(){std::cout <<"x:" << getX() << " and y:" << getY() << std::endl;}
    std::vector<double> toVector(){
        std::vector<double> vec;
        vec.push_back(x);
        vec.push_back(y);
        return vec;
    }
};
struct Part{
    Part(): weight(2){};
    Point p;
    double weight;
    operator double() const {return weight;}
    operator Point() const {return p;}
    void setWeight(double _w){ weight=_w;}
    void setPoint(Point _p){p=_p;}
    void print(){ std::cout << "Particle current weight " <<(double)*this << " with positional info of "; p.print();  }
};

#endif // STRUCTS_H_INCLUDED
