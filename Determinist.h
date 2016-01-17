#ifndef DETERMINIST_H
#define	DETERMINIST_H

#include "Parser.h"

class Determinist {
public:
    Determinist();
    Determinist(const Determinist& orig);
    virtual ~Determinist();
    
    void solve(Parser &p);
    
private:

};

#endif	/* DETERMINIST_H */

