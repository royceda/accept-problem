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


IloNumArray SubProb::getD(){
  return _D;
}

IloNum SubProb::getd(){
  return _d;
}




SubProb::SubProb(IloEnv &env1, IloNumArray x, IloNum theta, Parser &p):env(env1), _x(x),
_theta(theta), D(p.capacity()), n(p.nbCommands()), S(p.nbScenario()), _e(0), d(p.durationScenarTask()),
proba(p.probaVector()), sub(p.subTreatedCost()){


//  IloEnv env(env1);
  IloArray<IloNumVarArray> ytmp(env, S);
  y = ytmp;
  IloArray<IloNumArray> pitmp(env,S);
  pi = pitmp;
  IloArray<IloNumArray> primaltmp(env,S);
  primal = primaltmp;
  IloArray<IloRangeArray> constraintsTmp(env,S);
  constraints = constraintsTmp;

  /*initilization for each scenario*/
  for (int i = 0; i < S; i++) {
    cout<<i<<"\n";
    y[i]      = IloNumVarArray(env, n, 0, 1, ILOFLOAT);
    cout<<"y done\n";
    pi[i]     = IloNumArray(env, 2*n); //car 2n contrainte
    cout<<"pi done\n";
    primal[i] = IloNumArray(env, n);
    cout<<"primal done\n";
    model.push_back(IloModel(env));
    cout << "endOf "<<i<<"\n";
    constraints[i] = IloRangeArray(env);
  }
}

// bool SubProb::K2Test(){
//   //it exists an y >= 0 ???
//   for(int s = 0; s <S; s++){
//     double sum1 = 0;
//     for( int  i = 0; i<n; i++){
//       sum1 +=  d[s][i] * (_x[i] - y[s][i]);
//     }
//     if(sum1 > D){
//       return false;
//     }

//   }
//   for(int i = 0; i < n; i++){
//     if(_x[i] < 0){
//       return false;
//     }
//   }
//   return true;
// }


// void SubProb::feasibleCut(){
//   cout << "IN FEASIBLE CUT\n";
//   //creation of D and d from theta
//   for(int k = 0; k < S; k++){
//     IloNum pid(0);
//     for(int i = 0; i < n; i++){ //vector prod
//       pid += _theta * D;
//     }
//     _d += pid;
//     pid = 0;
//   }

//   cout << "AFTER 1\n";
//   //il y a 2n var dual car 2n contrainte

//   IloNumArray pit(env,n,0,IloInfinity, ILOINT);
//     for(int j = 0; j < n; j++){//matrix prod part 1
//       pit[i] +=  d[i][j] * _theta;
//     }
//     cout<<"AFTER2\n";
//     for(int j = 0; j < n; j++){//matrix prod part2
//       pit[i] +=  _theta * 1; //row x col2
//     }
//     pit = 0;
//     cout<<"AFTER2\n";

//     for(int k = 0; k < S; k++){//sum with k n part 1
//       _D[i] += proba[k] * pit[k] ;
//     }

//     cout<<"AFTER2\n";
//   }



  bool SubProb::optimTest(){
    IloNum w(0);
    IloNum a(0);

    IloNumArray tmpE(env,n);
    _E = tmpE;

    for(int i=0; i<n; i++){
      a += _E[i] * _x[i];
    }

    cout << "for done\n";

    w = a - _e;

    if( _theta >= w){
      cout<<"OPTIM TEST = TRUE\n";
      return true;
    }else{
      cout<<"OPTIM TEST = FALSE\n";
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
          e0 += y[s][i] * sub[i];
        }

        IloObjective obj(env, e0, IloObjective::Maximize, "OBJ");
        e0.end();
        model[s].add(obj);

        IloExpr e1(env);
        for (int i = 0; i < n; i++) { // matrix prod
          e1 += d[i][s] * (_x[i] - y[s][i]); //d is vector of demands
        }

        constraints[s].add(e1 <= D); // D is the total duration
        e1.end();

      #pragma omp parrallel for
      for (int i = 0; i < n; i++) { //second part: n constraints
        constraints[s].add(y[s][i] <= _x[i]); // [order][scenar]
      }
      model[s].add(constraints[s]);
      IloCplex cplex(model[s]);
      cplex.solve();
      cplex.getValues(primal[s], y[s]);
      //cplex.getDuals(pi[s], constraints[s]);
      cplex.end();
    }

  }catch (IloException& e) {
    cerr << "ERROR : " << e << "\n";
  }
}



void SubProb::optimalCut(){

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
  /* IloExpr cut(env);

  for(int k = 0; k < n; k++ ){
    cut -=_E[k] * x[k];
  }

  cut += _e; //>= theta
  return cut;*/
}


/**
 *
 * @return Generate the new cut from the sub problem
 */
bool SubProb::solve(Parser &p, IloNumVarArray x){
  IloExpr cut(env);
  // if(!K2Test()){ //Recours complet donc forcément dans K2
  //   cout<<"!K2Test\n";
  //   feasibleCut();
  //   return 0;
  // }

    initScenario(); //Traitement des sous problèmes
    cout << "inti donnennnne\n";
    if(!optimTest()){
      cout<<"!optimTest\n";
      optimalCut();
      return true;
    }
    return false;  

  }
