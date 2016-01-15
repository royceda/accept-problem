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
  IloNumArray tmpE(env,n);
  _E = tmpE;
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
    y[i]      = IloNumVarArray(env, n, 0, 1, ILOFLOAT);
    pi[i]     = IloNumArray(env, n+1); //car 2n contrainte
    primal[i] = IloNumArray(env, n);
    model.push_back(IloModel(env));
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


  for(int i=0; i<n; i++){
    a += _E[i] * _x[i];
  }
  w = _e - a;
  cout<<"w = "<<w<<"\n";
  cout <<"theta = "<<_theta<<"\n";
  if( _theta <= w){
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

      e0 = e0*proba[s];

      IloObjective obj(env, -e0, IloObjective::Maximize, "OBJ");
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
      cplex.getDuals(pi[s], constraints[s]);
      cplex.end();
      //cout<<"pi = "<<pi<<"\n";
    }

  }catch (IloException& e) {
    cerr << "ERROR : " << e << "\n";
  }
}

void SubProb::optimalCut(){
  cout<<"IN OPtimal\n";
  /*creation of E and e from pi*/
  for(int k = 0; k < S; k++){
    IloNum pid(0);
    pid += pi[k][0]*D;
    for(int i = 1; i < n+1; i++){ //vector prod
      pid += pi[k][i];
    }
    _e += proba[k] * pid ;
    pid = 0;
  }
  //cout<<"size pit : "<<pi.getSize()<<"\n";
  //cout<<"size pi[0] = "<<pi[0].getSize()<<"\n";
  //il y a 2n var dual car 2n contrainte
  IloNumArray pit(env,n);

  for(int i = 0; i < n; i++){
    IloNum sum(0);
    for(int j = 0; j<S; j++){
      for(int ii = 1; ii<n+1; ii++){
        sum -= pi[j][ii];
      }
      _E[i] += (d[i][j]*pi[j][0])*proba[j];
    }
  }
  // for(int i = 0; i<n; i++){
  //   cout << "E["<<i<<"]="<<_E[i]<<"\n";
  // }

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
    optimalCut();
    if(!optimTest()){
      cout<<"!optimTest\n";
      return true;
    }
    return false;  

  }
