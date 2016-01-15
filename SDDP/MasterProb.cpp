/* 
 * File:   MasterProb.cpp
 * Author: rboudjeltia
 * 
 * Created on 18 décembre 2015, 11:01
 */

#include "MasterProb.h"
#include <iostream>

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

    IloNum  MasterProb::Theta(){
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
        char nameX[256];
        for (int i = 0; i < p.nbCommands(); i++) {
            
            sprintf(nameX, "x %d ", i);
            x[i] = IloNumVar(env, 0, 1, ILOBOOL, nameX);
        }
        cout <<"nbC : "<< p.nbCommands()<<"\n";
        cout <<"x: "<<x<<"\n";

        char nameTheta[256];
        sprintf(nameTheta, "theta");
        IloNumVar theta(env, -IloInfinity, 0, ILOINT,nameTheta);

        IloExpr eObj1(env);

        for (int i = 0; i < p.nbCommands(); i++) {
            eObj1 += (p.benefVector()[i] * x[i]);
        }
        cout << "BENEF ::::::::::::: "<<eObj1<<"\n";
        IloExpr eObj2(env);
        eObj2 += theta;

        cout << "eObj2"<<eObj2<<"\n";

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
        IloRangeArray lazy;
        bool newOrNot = true;
        int count = 0;
        while(newOrNot){
            //cout<< "MODEL : "<<model<<"\n";

            IloCplex cplexMaster(model);
            cplexMaster.solve();

            cout << "MODEL : "<< model <<"\n";

            cout <<"\n\nMASTER SOL = " <<cplexMaster.getObjValue()<<"\n\n";
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
                    newConstraint -= subProb->getE()[i]*x[i];
                }
                newConstraint += subProb->gete();
                //IloNumVar theta(env, -IloInfinity, 0, ILOINT);
                //lazy.add(newConstraint >= theta);
                //cplexMaster.addLazyConstraints (lazy);
                //cplexMaster.LazyConstraintCallback();
                //cplexMaster.end();
                model.add(newConstraint >= theta);
                cout<<"NEW CUT ADDED"<<theta<<" <= "<<newConstraint<<"\n";
                cplexMaster.end();
            }

            else    
                break;
            // count ++;
            // cout <<"\n\nSOL= " <<cplexMaster.getObjValue()<<"\n\n";
            // for (int i = 0; i < p.nbCommands(); i++) {
            //     cout << "x" << i << " = " << cplexMaster.getValue(x[i]) << "\n";
            // }
            count++;
        }

    } catch (IloException& e) {
        cerr << "EEXECPTION CATCHED WHILE CPLEXING MASTER : " << e << "\n";
    }

}