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
 IloNumArray   MasterProb::x(){
        return _x;
    }

IloNum  MasterProb::theta(){
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
        IloNumVarArray x(env, p.nbCommands());

        //Initialize MasterProb
        cout<<"OKLM 1\n";
        
        for (int i = 0; i < p.nbCommands(); i++) {
            x[i] = IloNumVar(env, 0, 1, ILOBOOL);
        }

        IloNumVar theta(env, -IloInfinity, 0, ILOINT);

        IloExpr eObj(env);

        for (int i = 0; i < p.nbCommands(); i++) {
            eObj += (p.benefVector()[i] * x[i]);
        }

        eObj += theta;

        IloObjective obj(env, eObj, IloObjective::Maximize, "OBJ");
        model.add(obj);
                cout<<"OKLM 2\n";
        /*Contraintes initiales
         =>Y'en a pas*/

        /*For an iteration v (as in the book)*/
        int v = 0;
        IloExpr newConstraint(env);
        int newOrNot;
        int count = 0;
        while(count < 100){
            IloCplex cplexMaster(model);
            cplexMaster.solve();
cout<<"OKLM 1\n";

        IloNumArray _x(env, p.nbCommands());
        cplexMaster.getValues(_x,x);
        cout<<"OKLM 2\n";
        _theta = cplexMaster.getValue(theta);
cout<<"OKLM 1\n";
            SubProb *subProb = new SubProb(env,_x,_theta,p);
            newOrNot = subProb->solve(p,x);
            if(newOrNot == 0){ //Feasible Cut
                for(int i =0; i<p.nbCommands(); i++){
                    newConstraint += subProb->getD()[i]*x[i];
                }
                newConstraint += subProb->getd();
                model.add(theta<=newConstraint);
            }
            
            else if(newOrNot==1){ //Optimal Cut
                for(int i =0; i<p.nbCommands(); i++){
                    newConstraint += subProb->getE()[i]*x[i];
                }
                newConstraint += subProb->gete();
                model.add(theta<=newConstraint);
            }
        
        else if(newOrNot == 2)
            break;
        count ++;
                cout <<"\n\nSOL= " <<cplexMaster.getObjValue()<<"\n\n";
    }
        cout<<"OKLM 3\n";





} catch (IloException& e) {
    cerr << "EEXECPTION CATCHED WHILE CPLEXING MASTER : " << e << "\n";
}

}