/* 
 * File:   MasterProb.cpp
 * Author: rboudjeltia
 * 
 * Created on 18 décembre 2015, 11:01
 */

#include "MasterProb.h"

ILOSTLBEGIN

MasterProb::MasterProb() {
}

MasterProb::MasterProb(const MasterProb& orig) {
}

MasterProb::~MasterProb() {
}

IloExpr MasterProb::obj() {
    return _obj;
}

/*
SubProb MasterProb::sub() {
    return _sub;
}*/
MasterProb::x(){
    return _x;
}

MasterProb::theta(){
    return _theta;
}

/**
 * 
 * @param p
 */
void MasterProb::solve(Parser &p) {

    /*Define the master program from parser*/

    IloEnv env;
    try {
        IloModel model(env);
        IloArray<IloNumVar> x(env, p.nbCommands());

        //Initialize MasterProb
        sub(env);
        
        for (int i = 0; i < p.nbCommands(); i++) {
            x[i] = IloNumVar(env, 0, 1, ILOBOOL);
        }

        IloNumVar theta(env, -IloInfinity, 0, ILOINT);

        IloExpr eObj(env);

        for (int i = 0; i < p.nbCommands(); i++) {
            eObj += (p.benefVector()[i] * x[i]);
        }

        IloObjective obj(env, eObj, IloObjective::Maximize, "OBJ");
        model.add(obj);
        
        /*Contraintes initiales
         =>Y'en a pas*/

        /*For an iteration v (as in the book)*/
        int v = 0;
        IloExpr newConstraint(env);
        int newOrNot;
        while(1){
            IloCplex cplexMaster(model);
            cplexMaster.solve();

            cplexMaster.getValues(_x,x);
            cplexMaster.getValues(_theta,theta);

            SubProb *subProb = new SubProb(env,_x,_theta,p);
            newOrNot = subProb->solve(p,x);
            if(newOrNot == 0){ //Feasible Cut
                newConstraint += ;
                model.add(theta<=newConstraint);
            }
            else if(newOrNot==1){ //Optimal Cut
                newConstraint +=;
                model.add();
            }
            else if(newOrNot == 2)
                break;
        }

        /*solve the determinwist program x and theta are the optimal solution*/

        /*check the validity*/

        /*for each scenario*/
        /*Solve the sub porblem (primal and dual) (w & pi)*/

        //IloExpr cut = sub().solve(*env);

        /*add optimality cut El*x + theta > el*/
        //model.add(theta <= cut);
    } catch (IloException& e) {
        cerr << "EEXECPTION CATCHED WHILE CPLEXING MASTER : " << e << "\n";
    }

}