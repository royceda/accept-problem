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
  
    int s = p.nbScenario();
    int n = p.nbCommands();
    int D = p.capacity();
    
    
    IloArray <IloNumVarArray> y(env, n); //other commands
    
    /*initilization*/
    for (int i = 0; i < n; i++) {
      y[i] = IloNumVarArray(env, s, 0, 1, ILOBOOL);
    }

    
    for( int i = 0; i < s; i++){
      // model[i] = new IloModel(env);
    }
     
    //Primal solution of master problem
    IloArray<IloNumVar> x(env);

    //Dual solution of sub problem
    IloArray< IloArray <IloNumVar> >  pi(env);
     
     
    /*Definition of s subProblems*/
    for( int i = 0; i < s; i++){

      /*Objective*/
      IloExpr e0(env);
      for(int i = 0; i<n; i++){

	e0 += y[i][s] * p.subTreatedCost()[i]; ; 
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
    /**
     * TODO 
     * get primal solution y
     * get dual solution pi
     */
    for( int i = 0; i < s; i++){
      IloCplex cplex(model[i]);
      cplex.exportModel("test.lp");
      //x = cplex.solve();



      //check the syntax
      if(cplex.isDualFeasible()){
	// IloNumVarArray duals(env);
	//cplex.getDuals(duals, range); //avec un iloRange
	     
      }
    }
     
     
    /*creation of E and e from pi*/
    IloExpr e(env); 
    for( int i = 0; i < s; i++){
      for(int k = 0; k < n; k++ )
	e += p.probaVector()[i] * pi[i][k] * D;         
    }
     
     
    IloExpr E(env);
    for( int i = 0; i < s; i++){
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
  return NULL;
}

