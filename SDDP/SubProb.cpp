/* 
 * File:   SubProb.cpp
 * Author: rboudjeltia
 * 
 * Created on 18 décembre 2015, 11:00
 */

#include "SubProb.h"

SubProb::SubProb() {}

SubProb::SubProb(IloEnv &env): _env(env) {}

SubProb::SubProb(const SubProb& orig) {}

SubProb::~SubProb() {}



/**
 * 
 * @return Generate the new cut from the sub problem
 */
IloExpr SubProb::solve(Parser &p){

  IloEnv env;
  
  try{
    vector<IloModel> model;
  
    int S = p.nbScenario();
    int n = p.nbCommands();
    int D = p.capacity();
    
    
    IloArray <IloNumVarArray> y(env, S); //other commands
    IloNumArray  x(env);//Primal solution of master problem (given)
    IloArray <IloNumArray> primal(env, S); //primal values of subprob
    IloArray <IloNumArray> pi(env, S); //dual values of subprob
    
     IloArray <IloRangeArray>  constraints(env); //Constraints
    
    
    /*initilization*/
    for (int i = 0; i < S; i++) {
      y[i] = IloNumVarArray(env, S, 0, 1, ILOBOOL);
    }

    
    for( int i = 0; i < S; i++){
      pi[i] = IloNumArray(env, n);
      primal[i] = IloNumArray(env, n);
        
    }
     
    
     
  
     
     
     
     
     
     
     
    /*Definition of s subProblems for each scenario*/
    for( int s = 0; s < S; s++){

      /*Objective*/
      IloExpr e0(env);
      for(int i = 0; i<n; i++){
          e0 += y[i][s] * p.subTreatedCost()[i]; ; 
      }
      IloObjective obj(env, e0, IloObjective::Maximize, "OBJ");
      model[s].add(obj);
      
      
    
      
      //for (unsigned int k = 0; k < s; k++) {
      IloExpr e1(env);
      for (unsigned int i = 0; i < n; i++) {
          e1 += p.durationScenarTask()[i][s] * (x[i] - y[i][s]); //d is vector of demands
      }
      constraints[s].add(e1 <= D); // D is the total duration
      e1.end();
      //}
      
      
      for (unsigned int i = 0; i < n; i++) {
          //for (unsigned int j = 0; j < s; s++) {
          IloExpr tmp(env);
          tmp = y[i][s];
          
          constraints[s].add(y[i][s]<= x[i]); // [order][scenar]
          tmp.end();
          //}
      }
      model[s].add(constraints[s] );
    }
    
     
     
    
    /* resolve subProb and dual*/
    /**
     * TODO 
     * get primal solution y
     * get dual solution pi
     */
    for( int s = 0; s < S; s++){
        //getvalues sol
      IloCplex cplex(model[s]);
      cplex.getValues(primal[s], y[s]);
      cplex.getDuals(pi[s], constraints[s]);
        
    }
     constraints.end();
    
    
    
    /*creation of E and e from pi*/
    IloExpr e(env); 
    for( int i = 0; i < S; i++){
      for(int k = 0; k < n; k++ )
	e += p.probaVector()[i] * pi[i][k] * D;         
    }
     
     
    IloExpr E(env);
    for( int i = 0; i < S; i++){
      for(int k = 0; k < n; k++ ){
	E += p.probaVector()[i] * pi[i][k] * p.durationScenarTask()[i][k];                
      }
    }
     
    /*check theta validity*/
    /*check theta validity*/
    /*stop if theta > w*/
     
     
    /*cut creation*/
    IloExpr cut(env);

    for(int k = 0; k < n; k++ ){
      cut = e - E*x[k];
    }
    return cut; // just to trace the generation
  }catch (IloException& e) {
    cerr << "ERROR : " << e << "\n";
  }
  env.end();
  return NULL; //return an iloRangeArray which is the cut
}

