/* 
Authors: 
Wisam Reid cs184-ds
Eddie Groshev cs184-en
*/

#include "vertex.h"

// stl :: String Theory Labs (Taking over a universe near you)
using namespace stl;

Vertex::Vertex(const Point& p_, const Point& n_) : p(p_), n(n_), u(-1), v(-1) {}
Vertex::Vertex(const Point& p_, const Point& n_, double u_, double v_) : p(p_), n(n_), u(u_), v(v_) {}
const Point& Vertex::pos() {return p;}
const Point& Vertex::normal() {return n;}
const double Vertex::getU() {return u;}
const double Vertex::getV() {return v;}

// io stuff
void Vertex::stream_to(std::ostream& os) const {os<< "P: " << p << " N: " << n;}
