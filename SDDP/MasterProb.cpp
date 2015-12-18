/* 
 * File:   MasterProb.cpp
 * Author: rboudjeltia
 * 
 * Created on 18 décembre 2015, 11:01
 */

#include "MasterProb.h"

MasterProb::MasterProb() {
}

MasterProb::MasterProb(const MasterProb& orig) {
}

MasterProb::~MasterProb() {
}

IloExpr MasterProb::obj(){
    return _obj;
}

SubProb MasterProb::sub(){
    return _sub;
}


/**
 * 
 * @param p
 */
void MasterProb::solve(Parser &p){
    
    /*Define the master program from parser*/
    
    /*For an iteration v (as in the book)*/
    
    /*solve the determinist program x and theta are the optimal solution*/
    
    /*check the validity*/
    
    /*for each scenario*/
    /*Solve the sub porblem (primal and dual) (w & pi)*/
   
     IloExpr cut = sub().solve(); 
    
    /*add optimality cut El*x + theta > el*/
    
}