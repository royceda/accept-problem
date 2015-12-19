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
    IloEnv env;
    vector<IloModel> model;
    
    
    
    IloExpr cut(env);

    int s = p.nbScenario();
    int n = p.nbCommands();
    int D = p.capacity();
    IloExpr E(env);
    
     IloArray <IloNumVarArray> y(env, n); //other commands
    
     /*initilization*/
     for (int i = 0; i < n; i++) {
         y[i] = IloNumVarArray(env, s, 0, 1, ILOBOOL);
     }
     
     for( int i = 0; i < s; i++){
         model[i] = new IloModel(env);
     }
     
     //Primal solution of master problem
     IloNumVarArray<IloNumVar> x(env);
     //Dual solution of sub problem
     IloNumVarArray<IloNumVar> pi(env);
     
     
     
     /*Definition of s subProblems*/
     for( int i = 0; i < s; i++){
         /*Objective*/
         IloExpr e0(env);
         for(int i = 0; i<n; i++){
             e0 += p.subTreatedCost() * y[i][s]; 
         }
        
         IloObjective obj(env, e0, IloObjective::Maximize, "OBJ");
         model[s].add(obj);
        /*Constraints*/
         for (unsigned int j = 0; j < s; s++) {
            IloExpr e1(env);
            for (unsigned int i = 0; i < n; i++) {
                e1 += p.durationScenarTask()[i][j] * (x[i] - y[i][j]); //d is vector of demands
            }
            model[s].add(e1 <= D); // D is the total duration
        }
        
         for (unsigned int i = 0; i < n; i++) {
             for (unsigned int j = 0; j < s; s++) {
                 model[s].add(y[i][j] <= x[i]);
             }
         }
        
        
        
       
           
        
    }
    
    
     /* resolve subProb and dual*/
    /*check theta validity*/
    /*stop if theta > w*/
    /*cut creation*/
    
    /*creat E and e from pi*/
    
    //cut = e - E*x;
    return cut; // just to trace the generation
}

