#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include "include/preprocessing.hpp"
#include "include/twoPass.hpp"


using namespace std;


int main(int argc, char **argv){

    string flagOption = argv[1];
    string filename = argv[2];

    PreProcessing *preprocessing = new PreProcessing();
    TwoPass *twoPass = new TwoPass();

    if (flagOption == "-p") {
        preprocessing->runPreProcessing(filename);        
    }
    if (flagOption == "-o") {
        twoPass->runTwoPassAlgorithm(filename);
    }

    return 0;

}