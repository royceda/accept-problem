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

ILOSTLBEGIN
        
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    IloEnv env;
    Parser* p = new Parser("./../Instances/data_O5/OA_O5_S3_L0.2_B10_R5_0.txt");
    cout << "Parsing done\n";
    p->printParser();
    env.end();
    
    return 0;
}

