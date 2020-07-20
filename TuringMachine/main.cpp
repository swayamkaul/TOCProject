#include "turingMachineAdd.h"
#include "turingMachineComparator.h"
#include "turingMachineCopy.h"
#include "turingMachineMultiply.h"
#include <fstream>

int nm(std::deque<char> & dq){
    int ans = 0;
    int mul = 1;
    int st, en;
    for(int i = 0; i < dq.size(); i++){
        if(*(dq.begin()+i)=='^'){st=i+1;}
        if(*(dq.begin()+i)=='|'){en=i;break;}
    }
    for(int i = en-1; i >=st; i--){
        ans+=(*(dq.begin()+i)=='1')*mul;
        mul*=2;
    }
    return ans;
}

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
    std::deque<char> magicNo = {'^','1','0','0','0','0','0','0','0','0','|'};
    TuringMachineMultiply TMMulTh(magicNo,threshhold);
    TMMulTh.simulate();
    int counter = 0;
    int blackPixels = 0;
    while(!(t0.eof() || t1.eof() || t2.eof())){
        std::deque<char> tape0(24);
        std::deque<char> tape1(24);
        std::deque<char> tape2(24);
        std::string s;
        t0 >> &tape0[0];
        tape0.push_front('^');
        tape0.push_back('|');
        t1 >> &tape1[0];
        tape1.push_front('^');
        tape1.push_back('|');
        t2 >> &tape2[0];
        tape2.push_front('^');
        tape2.push_back('|');
        if((t0.eof() || t1.eof() || t2.eof())){break;}
        std::deque<char> magicValR = {'^','1','0','1','1','1','0','1','0','1','1','0','1','|'};
        std::deque<char> magicValG = {'^','1','0','1','1','0','1','1','1','0','1','1','1','0','|'};
        std::deque<char> magicValB = {'^','1','0','0','0','1','1','1','0','1','0','0','|'};
        std::string st;
        TuringMachineMultiply TMmulR(magicValR,tape0);
        TMmulR.simulate();

        TuringMachineMultiply TMmulG(magicValG,tape1);
        TMmulG.simulate();

        TuringMachineMultiply TMmulB(magicValB,tape2);
        TMmulB.simulate();

        TuringMachineAdd TMAddRG(tape0, tape1);
        TMAddRG.simulate();

        TuringMachineAdd TMAddRGB(tape1, tape2);
        TMAddRGB.simulate();

        TuringMachineComparator TMComp(threshhold,tape2);
        TMComp.simulate();
        counter++;
        blackPixels+=(tape2.front()=='1');
    }
    std::ofstream output("../percentage.txt");
    output << (1.0-(blackPixels*1.0)/(counter*1.0))*100.0;
    std::cerr << "Number of black pixels: " << blackPixels << '\n';
    std::cerr << "Number of white pixels: " << counter - blackPixels << '\n';
    std::cerr << "Number of pixels processed: " <<  counter << '\n';
    std::cerr << "Percentage of white pixels" << (1.0-(blackPixels*1.0)/(counter*1.0))*100.0 << '\n';
}
