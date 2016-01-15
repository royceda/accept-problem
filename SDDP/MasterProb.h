/* 
 * File:   MasterProb.h
 * Author: rboudjeltia
 *
 * Created on 18 décembre 2015, 11:01
 */

#ifndef MASTERPROB_H
#define	MASTERPROB_H

#include "Parser.h"
#include "SubProb.h"

#include <ilcplex/ilocplex.h>


class MasterProb {
public:
    MasterProb();
    MasterProb(const MasterProb& orig);
    virtual ~MasterProb();
    
//    SubProb sub();
    IloExpr obj();
    void solve(Parser &p);  

    IloNumArray x();
    IloNum Theta();

private:
//    SubProb _sub;
    IloExpr _obj;
    IloNumArray _x;
    IloNum _theta;

};

#endif	/* MASTERPROB_H */

