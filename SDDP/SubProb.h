/*
 * File:   SubProb.h
 * Author: rboudjeltia
 *
 * Created on 18 décembre 2015, 10:59
 */


/**
*TODO
* feasibible cut
* K2 test
*/
#ifndef SUBPROB_H
#define	SUBPROB_H

#include "ilcplex/ilocplex.h"

#include "Parser.h"

class SubProb {

public:
  SubProb();
  SubProb(IloEnv &env1);
  SubProb(IloEnv &env1, IloNumArray x, IloNum theta, Parser &p); //done
  SubProb(const SubProb& orig);
  virtual ~SubProb();

  IloExpr solve(Parser &p, IloArray<IloNumVar> x);
  IloExpr optimalCut(IloArray<IloNumVar> x); 
  IloExpr feasibleCut(IloArray<IloNumVar> x); //TODO

  IloNumArray getE();
  IloNum gete();


protected:
  bool K2Test(); //TODO
  void initScenario();
  bool optimTest();


private:
  IloEnv env;
  IloNum _theta;
  IloNumArray _x;

  //vector<IloObjective> obj;
  vector<IloModel> model;
  IloArray <IloRangeArray> constraints;
  IloArray <IloNumArray>  primal; //primal values of subprob
  IloArray <IloNumArray> pi;  //dual values of subprob
  IloArray <IloNumVarArray> y; //other commands

  IloNumArray _E;
  IloNum _e;


  int S; // nb scenario
  int n; // nb Comands
  int D; // Capacity
  vector<int> sub; //subTreatedCost
  vector<vector<int> > d; //demands a verif
  vector<double> proba;

};

#endif	/* SUBPROB_H */
