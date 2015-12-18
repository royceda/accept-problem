/* 
 * File:   SubProb.h
 * Author: rboudjeltia
 *
 * Created on 18 décembre 2015, 10:59
 */

#ifndef SUBPROB_H
#define	SUBPROB_H

#include "ilcplex/ilocplex.h"

#include "Parser.h"

class SubProb {
public:
    SubProb();
    SubProb(const SubProb& orig);
    virtual ~SubProb();
    
    IloExpr solve(Parser &p); 
    
private:
    
};

#endif	/* SUBPROB_H */

