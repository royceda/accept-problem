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

SubProb MasterProb::sub() {
    return _sub;
}

/**
 * 
 * @param p
 */
void MasterProb::solve(Parser &p) {

    /*Define the master program from parser*/

    IloEnv env();
    try {
        IloModel model(env);
        IloArray<IloNumVar> x(env, p.nbCommands());

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
        while(/*(Pas de nvelles coupes) ou (zSup - zInf > eps) ou (pas d'amélioration de zSup significative)*/){
            
            v++;
        }

        /*solve the determinist program x and theta are the optimal solution*/

        /*check the validity*/

        /*for each scenario*/
        /*Solve the sub porblem (primal and dual) (w & pi)*/

        IloExpr cut = sub().solve(*env);

        /*add optimality cut El*x + theta > el*/
        model.add(theta <= cut);
    } catch (IloException& e) {
        cerr << "EEXECPTION CATCHED WHILE CPLEXING MASTER : " << e << "\n";
    }

}