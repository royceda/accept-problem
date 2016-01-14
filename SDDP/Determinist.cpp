/* 
 * File:   Determinist.cpp
 * Author: rboudjeltia
 * 
 * Created on 16 décembre 2015, 14:44
 */

#include <ilcplex/ilocplex.h>
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
        IloNumVarArray x(env, n); //accepted commands
        IloArray <IloNumVarArray> y(env, n); //other commands



        /**init values*/
        for (int i = 0; i < n; i++) {
            x[i] = IloNumVar(env, 0, 1, ILOBOOL);
            y[i] = IloNumVarArray(env, s, 0, 1, ILOBOOL);
        }


        /**Objectif*/
        IloExpr e0(env);
        for (unsigned int i = 0; i < n; i++) {
            e0 += p.benefVector()[i] * x[i];
        }


        IloExpr e1(env);
        for (unsigned int j = 0; j < s; j++) {
            IloExpr e12(env);
            for (unsigned int i = 0; i < n; i++) {
                e12 += p.probaVector()[j] * p.subTreatedCost()[i] * y[i][j];
            }
            e1 += p.probaVector()[j] * e12;
            e12.end();
        }

        cout << "Obj : "<<e0 - e1<<"\n"; 
        IloObjective obj(env, e0 - e1, IloObjective::Maximize, "OBJ");
        model.add(obj);
        e0.end();
        e1.end();

        /**Contraintes*/
        for (unsigned int j = 0; j < s; j++) {
            IloExpr e1(env);
            for (unsigned int i = 0; i < n; i++) {
                e1 += p.durationScenarTask()[i][j] * (x[i] - y[i][j]); //d is vector of demands
            }

            model.add(e1 <= IloInt(D)); 
            e1.end();// D is the total duration
        }

        for (unsigned int i = 0; i < n; i++) {
            for (unsigned int j = 0; j < s; j++) {
                IloExpr exp1(env);
                IloExpr exp2(env);
                exp1 += y[i][j];
                exp2 += x[i];
                model.add(exp1 <= exp2);
                exp1.end();
                exp2.end();
            }
        }


        IloCplex cplex(model);
        cplex.solve();
        cout <<"\n\nSOL= " <<cplex.getObjValue()<<"\n\n";

    } catch (IloException &e) {
        cerr << "EXECPTION CAUGHT WHILE CPLEXING\n"<<e<<"\n";
    }


}
