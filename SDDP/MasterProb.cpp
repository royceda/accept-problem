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
    
    
    
    
}