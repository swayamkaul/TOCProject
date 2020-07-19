#include "turingMachineAdd.h"
#include "turingMachineComparator.h"
#include "turingMachineCopy.h"
#include "turingMachineMultiply.h"
#include <fstream>

int main(){
    std::ifstream t0("tape0"), t1("tape1"), t2("tape2");
    std::ifstream dt("val");
    int val = 5000;
    std::deque<char> threshhold;
    threshhold.push_front('|');
    while(val){
        threshhold.push_front(val%2+'0');
        val/=2;
    }
    threshhold.push_front('^');
    int counter = 0;
    int blackPixels = 0;
    while(!(t0.eof() || t1.eof() || t2.eof())){
        std::deque<char> tape0(26);
        std::deque<char> tape1(26);
        std::deque<char> tape2(26);
        t0.getline(&tape0[0],26);
        t1.getline(&tape1[0],26);
        t2.getline(&tape2[0],26);
        std::deque<char> magicValR = {'^','1','0','1','1','1','0','1','0','1','1','0','1','|'};
        std::deque<char> magicValG = {'^','1','0','1','1','0','1','1','1','0','1','1','1','0','|'};
        std::deque<char> magicValB = {'^','1','0','0','0','1','1','1','0','1','0','0','|'};
        TuringMachineMultiply TMmulR(magicValR,tape0);
        TMmulR.simulate();
        TuringMachineMultiply TMmulG(magicValR,tape1);
        TMmulG.simulate();
        TuringMachineMultiply TMmulB(magicValR,tape2);
        TMmulB.simulate();
        TuringMachineAdd TMAddRG(tape0, tape1);
        TMAddRG.simulate();
        TuringMachineAdd TMAddRGB(tape1, tape2);
        TMAddRGB.simulate();
        TuringMachineComparator TMComp(threshhold,tape2);
        TMComp.simulate();
        counter++;
        blackPixels+=(tape2[0]=='1');
    }
    std::ofstream output("../percentage.py");
    output << "percentage = " << (1.0-(blackPixels*1.0)/(counter*1.0))*100.0 << '\n';
}