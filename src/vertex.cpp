/* 
Authors: 
Wisam Reid cs184-ds
Eddie Groshev cs184-en
*/

#include "vertex.h"

// stl :: String Theory Labs (Taking over a universe near you)
using namespace stl;

Vertex::Vertex(const Point& p_, const Point& n_) : p(p_), n(n_) {}
const Point& Vertex::pos() {return p;}
const Point& Vertex::normal() {return n;}

// io stuff
void Vertex::stream_to(std::ostream& os) const {os<< "P: " << p << " N: " << n;}
