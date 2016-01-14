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
        for (int i = 0; i < p.nbCommands(); i++) {
            x[i] = IloNumVar(env, 0, 1, ILOBOOL);
        }
        cout <<"nbC : "<< p.nbCommands()<<"\n";
        cout <<"x: "<<x<<"\n";

        IloNumVar theta(env, -IloInfinity, 0, ILOINT);

        IloExpr eObj1(env);

        for (int i = 0; i < p.nbCommands(); i++) {
            eObj1 += (p.benefVector()[i] * x[i]);
        }

        IloExpr eObj2(env);
        eObj2 += theta;


        IloExpr eObj(env);
        eObj = eObj1 + eObj2;

        cout << "OBJECTIVE : max "<< eObj <<"\n";

        IloObjective obj(env, eObj, IloObjective::Maximize, "OBJ");
        model.add(obj);
        /*Contraintes initiales
         =>Y'en a pas*/

        /*For an iteration v (as in the book)*/
        int v = 0;
        IloExpr newConstraint(env);
        bool newOrNot;
        int count = 0;
        while(count < 100){
            IloCplex cplexMaster(model);
            cplexMaster.solve();

            cout <<"\n\nMASTER SOL without sub = " <<cplexMaster.getObjValue()<<"\n\n";
            for (int i = 0; i < p.nbCommands(); i++) {
                cout << "x" << i << " = " << cplexMaster.getValue(x[i]) << "\n";
            }

            IloNumArray _x(env, p.nbCommands());
            cplexMaster.getValues(_x,x);
            _theta = cplexMaster.getValue(theta);
            SubProb *subProb = new SubProb(env,_x,_theta,p);
            newOrNot = subProb->solve(p,x);
            // if(newOrNot == 0){ //Feasible Cut       //Always in K2 because of a complete recours
            //     for(int i =0; i<p.nbCommands(); i++){
            //         newConstraint += subProb->getD()[i]*x[i];
            //     }
            //     newConstraint += subProb->getd();
            //     model.add(theta<=newConstraint);
            // }
            
            if(newOrNot){ //Optimal Cut
                cout << "NEW CUT TO ADD\n";
                for(int i =0; i<p.nbCommands(); i++){
                    newConstraint += subProb->getE()[i]*x[i];
                }
                newConstraint += subProb->gete();
                IloNumVar theta(env, -IloInfinity, 0, ILOINT);
                model.add(theta<=newConstraint);
                cout<<"NEW CUT ADDED"<<newConstraint<<"\n";
            }

            else    
                break;
            count ++;
            cout <<"\n\nSOL= " <<cplexMaster.getObjValue()<<"\n\n";
            for (int i = 0; i < p.nbCommands(); i++) {
                cout << "x" << i << " = " << cplexMaster.getValue(x[i]) << "\n";
            }
            count++;
        }

    } catch (IloException& e) {
        cerr << "EEXECPTION CATCHED WHILE CPLEXING MASTER : " << e << "\n";
    }

}