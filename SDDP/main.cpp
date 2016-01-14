/* 
 * File:   main.cpp
 * Author: rboudjeltia
 *
 * Created on 11 décembre 2015, 10:29
 */

#include <cstdlib>
#include <iostream>

#include "ilcplex/ilocplex.h"
#include "Parser.h"
#include "Determinist.h"
#include "MasterProb.h"

ILOSTLBEGIN
        
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    IloEnv env;
    Parser* p = new Parser("./../Instances/data_O10/OA_O10_S10_L0.5_B10_R10_3.txt");
    cout << "Parsing done\n";
    p->printParser();
    Determinist * det = new Determinist();
    det->solve(*p);

    MasterProb* MP = new MasterProb();
    MP->solve(*p);

    env.end();
    
    return 0;
}

