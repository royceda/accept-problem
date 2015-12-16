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
    unsigned int n = p.nbCommands();
    unsigned int s = p.nbScenario();
    
    try{
    IloModel model(env);

        /**Variables*/
    IloArray <IloNumVarArray> x(env, n);
    IloArray < IloNumVarArray> y(env, n);
    
    /**init values*/
    
    for(unsigned int i =0; i < n; i++){
        
        x[i] = IloNumVar(env, 0, 1, ILOBOOL);
        y[i] = IloNumVarArray(env, s, 0, 1, ILOBOOL);
 
    }
    
    
    
        /**Objectif*/
    IloExpr eO(env);
    for(unsigned int i = 0; i < n; i++){
        eO += p.benefVector()[i] * x[i]  
    }
    
    IloExpr e1(env);
    for(unsigned int i = 0; i < n; i++){
            for(unsigned int j = 0; j < s; j++){        
                e1 += p.probaVector()[j] * p.subTreatedCost()[i] * y[i][s];
        }
    }
    
    
    
    
    IloObjective obj(env, eO - e1, IloObjective::Maximize, "OBJ");      
    
        /**Contraintes*/
    
    for(unsigned int i = 0; i < n; i++){
        IloExpr e2(env);
        for(unsigned int i = 0; i < n; i++){
           // e2 += 
        }
    }
    
    
    
    }catch(IloException &e){
    }
    
    
}
