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
    SubProb(IloEnv &env);
    SubProb(const SubProb& orig);
    virtual ~SubProb();
    
    IloEnv env();
    
    IloExpr solve(Parser &p); 
    
    
private:
    IloEnv _env;
};

#endif	/* SUBPROB_H */

