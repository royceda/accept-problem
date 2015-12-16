/* 
 * File:   Determinist.cpp
 * Author: rboudjeltia
 * 
 * Created on 16 décembre 2015, 14:44
 */

#include "Determinist.h"

Determinist::Determinist() {
}

Determinist::Determinist(const Determinist& orig) {
}

Determinist::~Determinist() {
}

void Determinist::solve(Parser& p){
    IloEnv env;
    
    try{
    IloModel model(env);
    
    
    }catch(IloException &e){
    }
    
    
}
