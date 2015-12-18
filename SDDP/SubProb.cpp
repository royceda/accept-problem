/* 
 * File:   SubProb.cpp
 * Author: rboudjeltia
 * 
 * Created on 18 décembre 2015, 11:00
 */

#include "SubProb.h"

SubProb::SubProb() {
}

SubProb::SubProb(const SubProb& orig) {
}

SubProb::~SubProb() {
}

/**
 * 
 * @return Generate the new cut from the sub problem
 */
IloExpr SubProb::solve(Parser &p){
    IloExpr cut(env);

    
    
    
    /*check theta validity*/
    /*stop if theta > w*/
    /*cut creation*/
    
    /*creat E and e from pi*/
    
    cut = e - E*x;
    return cut; // just to trace the generation
}

