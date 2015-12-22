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

void Determinist::solve(Parser& p) {
    IloEnv env;
    unsigned int n = p.nbCommands();
    unsigned int s = p.nbScenario();
    unsigned int D = p.capacity();
    
    try {
        IloModel model(env);

        /**Variables*/
        IloArray <IloNumVarArray> x(env, n); //accepted commands
        IloArray <IloNumVarArray> y(env, n); //other commands



        /**init values*/
        for (int i = 0; i < n; i++) {
            x[i] = IloNumVar(env, 0, 1, ILOBOOL);
            y[i] = IloNumVarArray(env, s, 0, 1, ILOBOOL);
        }



        /**Objectif*/
        IloExpr eO(env);
        for (unsigned int i = 0; i < n; i++) {
            eO += p.benefVector()[i] * x[i];
        }

        IloExpr e1(env);
        for (unsigned int j = 0; j < s; j++) {
            IloExpr e12(env);
            for (unsigned int i = 0; i < n; i++) {
                e12 += p.probaVector()[j] * p.subTreatedCost()[i] * y[i][s];
            }
            e1 += p.probaVector()[j] * e12;
        }

        IloObjective obj(env, eO - e1, IloObjective::Maximize, "OBJ");


        /**Contraintes*/
        for (unsigned int j = 0; j < s; s++) {
            IloExpr e1(env);
            for (unsigned int i = 0; i < n; i++) {
                e1 += p.durationScenarTask()[i][j] * (x[i] - y[i][j]); //d is vector of demands
            }
            model.add(e1 <= IloInt(D)); // D is the total duration
        }

        for (unsigned int i = 0; i < n; i++) {
	  for (unsigned int j = 0; j < s; s++) {
	    IloExpr exp1(env);
	    IloExpr exp2(env);
	    exp1 = y[i][j];
	    exp2 = x[i];
	    model.add(exp1 <= exp2);
	  }
        }

        /**def y and x*/
        //Done !!!! ('_')

    } catch (IloException &e) {
        cerr << "EXECPTION CAUGHT WHILE CPLEXING\n"<<e<<"\n";
    }


}
