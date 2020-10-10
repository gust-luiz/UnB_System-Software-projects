#include "include/preprocessing.hpp"
#include "include/twoPass.hpp"


int main(int argc, char **argv){

    string flagOption = argv[1];
    string filename = argv[2];

    //string str = " IFT : iF ";
    //smatch matches;
    //regex reg;

    //regex_search(str, matches, reg = ("((^|:|\\s)IF(\\s))"));
    //regex_search(str, matches, reg = ("([: ](EQU ))"));
    //cout << matches.str() << endl;

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