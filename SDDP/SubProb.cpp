/*
 * File:   SubProb.cpp
 * Author: rboudjeltia
 *
 * Created on 18 décembre 2015, 11:00
 */

#include "SubProb.h"

SubProb::SubProb() {}

SubProb::SubProb(IloEnv &env1): env(env1) {}

SubProb::SubProb(const SubProb& orig) {}

SubProb::~SubProb() {
  constraints.end();
  model.end();
}

IloNumArray SubProb::getE(){
  return _E;
}

IloNum SubProb::gete(){
  return _e;
}


SubProb::SubProb(IloEnv &env1, IloNumArray x, IloNum theta, Parser &p):env(env1), _x(x),
_theta(theta), D(p.capacity()), n(p.nbCommands()), S(p.nbScenario()), y(env, S), _e(0),
primal(env, S), pi(env, S), constraints(env), d(p.durationScenarTask()),
proba(p.probaVector()), sub(p.subTreatedCost()){
  /*initilization for each scenario*/
  for (int i = 0; i < S; i++) {
    y[i]      = IloNumVarArray(env, S, 0, 1, ILOBOOL);
    pi[i]     = IloNumArray(env, 2*n); //car 2n contrainte
    primal[i] = IloNumArray(env, n);
    model[i]  = IloModel(env);
  }
}

bool SubProb::K2Test(){
  //it exists an y >= 0 ???
  for(int s = 0; s <S; s++){
    int sum1 = 0;
    for( int  i = 0; i<n; i++){
      sum1 +=  d[s][i] * _x[i];
    }

    if(sum1 > D){
      return false;
    }
  }
  for(int i = 0; i < n; i++){
    if(_x[i] < 0){
      return false;
    }
  }
  return true;
}


IloExpr SubProb::feasibleCut(IloArray<IloNumVar> theta){

  //creation of D and d from theta
  for(int k = 0; k < S; k++){
    IloNum pid(0);
    for(int i = 0; i < n; i++){ //vector prod
      pid += _theta * D;
    }
    _d += pid;
    pid = 0;
  }

/*
  //il y a 2n var dual car 2n contrainte
  for(int i = 0; i < S; i++){
    IloNumArray pit(0);
    for(int j = 0; j < n; j++){//matrix prod part 1
      pit[i] +=  d[i][j] * _theta;
    }

    for(int j = 0; j < n; j++){//matrix prod part2
      pit[i] +=  _theta * 1; //row x col2
    }
    pit = 0;
  }

    for(int k = 0; k < S; k++){//sum with k n part 1
      _D[i] += proba[k] * pit[k] ;
    }



    //cut creation
    IloExpr cut(env);

    for(int k = 0; k < n; k++ ){
      cut += _D[k] * theta ;
    }

    cut += -_d; //>= 0
    return cut;*/
  return NULL;
  }



bool SubProb::optimTest(){
  IloNum w(0);
  IloNum a(0);

  for(int i=0; i<n; i++){
    a += _E[i] * _x[i];
  }

  w = a - _e;

  if( _theta >= w){
    return true;
  }else{
    return false;
  }
}


void SubProb::initScenario(){
  try{
    /*step 3*/
    /*Definition of S subProblems for each scenario*/
    #pragma omp parrallel for
    for(int s = 0; s < S; s++){
      /*Objective*/
      IloExpr e0(env);
      for(int i = 0; i<n; i++){
        e0 += y[i][s] * sub[i]; ;
      }
      IloObjective obj(env, e0, IloObjective::Maximize, "OBJ");
      model[s].add(obj);


      IloExpr e1(env);

      for (int j = 0; j < n; j++) { // first part: n constraints
        for (int i = 0; i < n; i++) { // matrix prod
          e1 += d[i][s] * (_x[i] - y[i][s]); //d is vector of demands
        }
        constraints[s].add(e1 <= D); // D is the total duration
        e1.end();
      }

      #pragma omp parrallel for
      for (int i = 0; i < n; i++) { //second part: n constraints
        IloExpr tmp(env);
        tmp = y[i][s];

        constraints[s].add(y[i][s] <= _x[i]); // [order][scenar]
        tmp.end();
      }
      model[s].add(constraints[s]);
    }


    for(int s = 0; s < S; s++){
      IloCplex cplex(model[s]);
      cplex.getValues(primal[s], y[s]);
      cplex.getDuals(pi[s], constraints[s]);
    }
  }catch (IloException& e) {
    cerr << "ERROR : " << e << "\n";
  }
}



IloExpr SubProb::optimalCut(IloArray<IloNumVar> x){

  /*creation of E and e from pi*/
  for(int k = 0; k < S; k++){
    IloNum pid(0);
    for(int i = 0; i < n; i++){ //vector prod
      pid += pi[k][i] * D;
    }
    _e += proba[k] * pid ;
    pid = 0;
  }


  //il y a 2n var dual car 2n contrainte
  for(int i = 0; i < S; i++){
    IloNumArray pit(0);
    for(int j = 0; j < n; j++){//matrix prod part 1
      pit[i] +=  pi[i][j] * d[j][i];
    }

    for(int j = n; j < 2*n; j++){//matrix prod part2
      pit[i] +=  pi[i][j] * 1; //row x col2
    }

    for(int k = 0; k < S; k++){//sum with k n part 1
      _E[i] += proba[k] * pit[k] ;
    }
    pit = 0;


  }

  /*cut creation*/
  IloExpr cut(env);

  for(int k = 0; k < n; k++ ){
    cut -=_E[k] * x[k];
  }

  cut += _e; //>= theta
  return cut;
}


/**
 *
 * @return Generate the new cut from the sub problem
 */
IloExpr SubProb::solve(Parser &p, IloArray<IloNumVar> x){
  IloExpr cut(env);
  if(K2Test()){
    cut = feasibleCut(x);
    return cut;
  }
  else{
    initScenario();
    cut = optimalCut(x);
    return cut;
  }
  return NULL;
}
