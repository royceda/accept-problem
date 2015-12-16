/* 
 * File:   Determinist.cpp
 * Author: rboudjeltia
 * 
 * Created on 16 décembre 2015, 14:44
 */

#include "ilcplex/ilocplex.h"
#include "Determinist.h"

ILOSTLBEGIN

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

        /**Variables*/
    IloArray <IloNumVarArray> x(env, p.nbCommands());
    
    IloNumVarArray c(env, p.nbCommands());
    IloNumVarArray d(env, p.nbCommands());
    
    IloNumVarArray b(env, p.nbCommands());
    IloArray < IloNumVarArray> y(env, p.nbCommands());
    
    IloNumVarArray prob(env, p.nbScenario());
    
    
        /**Objectif*/
    
    
        /**Contraintes*/
    
    
    
    
    }catch(IloException &e){
    }
    
    
}
