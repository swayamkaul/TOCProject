#include "turingMachineMultiply.h"
#include "turingMachineAdd.h"
#include "turingMachineCopy.h"

TuringMachineMultiply::TuringMachineMultiply(std::deque<char> & tapeA,std::deque<char> &tapeB):m_tapeA(tapeA), m_tapeB(tapeB){
    presentState = e_states::init;
    m_headA=m_tapeA.begin();
    m_headB=m_tapeB.begin();
    m_memory.push_front('^');
    m_memory.push_back('|');
    m_states.push_back({e_stateControl::Start,{e_direction::Left,e_direction::Left},{'^','^'},{'n','n'},e_states::init,e_states::goToEnd});

    // Go to End
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Left},{'0','0'},{'n','n'},e_states::goToEnd,e_states::goToEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Left},{'0','1'},{'n','n'},e_states::goToEnd,e_states::goToEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Left},{'1','0'},{'n','n'},e_states::goToEnd,e_states::goToEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Left},{'1','1'},{'n','n'},e_states::goToEnd,e_states::goToEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Left},{'|','0'},{'n','n'},e_states::goToEnd,e_states::goToEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Left},{'|','1'},{'n','n'},e_states::goToEnd,e_states::goToEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Neutral},{'0','|'},{'n','n'},e_states::goToEnd,e_states::goToEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Neutral},{'1','|'},{'n','n'},e_states::goToEnd,e_states::goToEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Right,e_direction::Right},{'|','|'},{'n','n'},e_states::goToEnd,e_states::compute});

    // Computation begins.
    // 0 in operand
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'0','0'},{'n','|'},e_states::compute,e_states::appendZero});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'1','0'},{'n','|'},e_states::compute,e_states::appendZero});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'^','0'},{'n','|'},e_states::compute,e_states::appendZero});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'|','0'},{'n','|'},e_states::compute,e_states::appendZero});

    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Neutral},{'*','*'},{'n','n'},e_states::appendZero,e_states::appendZero1});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Neutral},{'*','*'},{'0','n'},e_states::appendZero1,e_states::appendZero2});
    m_states.push_back({e_stateControl::Normal,{e_direction::Right,e_direction::Neutral},{'*','*'},{'n','n'},e_states::appendZero2,e_states::compute});


    // 1 in operand
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'0','1'},{'n','|'},e_states::compute,e_states::add});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'1','1'},{'n','|'},e_states::compute,e_states::add});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'|','1'},{'n','|'},e_states::compute,e_states::add});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'^','1'},{'n','|'},e_states::compute,e_states::add});

    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'*','*'},{'n','n'},e_states::add,e_states::appendZero});

    // ^ in operand
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'0','^'},{'n','n'},e_states::compute,e_states::copy});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'1','^'},{'n','n'},e_states::compute,e_states::copy});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'|','^'},{'n','n'},e_states::compute,e_states::copy});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'^','^'},{'n','n'},e_states::compute,e_states::copy});

    m_states.push_back({e_stateControl::End,{e_direction::Neutral,e_direction::Neutral},{'*','^'},{'n','n'},e_states::copy,e_states::hlt});


}

int TuringMachineMultiply::p_findStateTransition(char sA, char sB){
    for(int i = 0; i < m_states.size(); i++){
        if((m_states[i].currentState == presentState) && (m_states[i].ruleInput[0] == sA || m_states[i].ruleInput[0] == '*') && (m_states[i].ruleInput[1]==sB || m_states[i].ruleInput[1]=='*')){
            return i;
        }
    }
    throw "Error";
}

void TuringMachineMultiply::p_emmit(int transitionId){
    if(m_states[transitionId].ruleOutput[0]!='n'){
        *m_headA = m_states[transitionId].ruleOutput[0];
    }
    if(m_states[transitionId].ruleOutput[1]!='n'){
        *m_headB = m_states[transitionId].ruleOutput[1];
    }
}

void TuringMachineMultiply::simulate(){
    while(presentState!=e_states::hlt){
        // Get input symbols from tape
        char sA = *m_headA;
        char sB = *m_headB;
        // Find transition
        // if(sB=='^'){
        //     std::cerr << "pika";
        // }
        int transitionIdx = p_findStateTransition(sA,sB);


        if(presentState==e_states::add){
            TuringMachineAdd tm(m_tapeA,m_memory);
            tm.simulate();
        }
        if(presentState==e_states::copy){
            TuringMachineCopy tm(m_memory,m_tapeB);
            tm.simulate();
        }

        // Process transition outputs
        p_emmit(transitionIdx);

        // Process tape movement
        if(m_states[transitionIdx].moveDirection[0]==e_direction::Neutral){

        }
        else if(m_states[transitionIdx].moveDirection[0]==e_direction::Left){
            if(m_headA==std::prev(m_tapeA.end())){
                m_tapeA.push_back('|');
                m_headA = std::prev(m_tapeA.end());
            }
            else{
                m_headA = std::next(m_headA);
            }
        }
        else if(m_states[transitionIdx].moveDirection[0]==e_direction::Right){
            if(m_headA==m_tapeA.begin()){
                m_tapeA.push_front('|');
                m_headA = m_tapeA.begin();
            }
            else{
                m_headA = std::prev(m_headA);
            }
        }

        if(m_states[transitionIdx].moveDirection[1]==e_direction::Neutral){

        }
        else if(m_states[transitionIdx].moveDirection[1]==e_direction::Left){
            if(m_headB==std::prev(m_tapeB.end())){
                m_tapeB.push_back('|');
                m_headB = std::prev(m_tapeB.end());
            }
            else{
                m_headB = std::next(m_headB);
            }
        }
        else if(m_states[transitionIdx].moveDirection[1]==e_direction::Right){
            if(m_headB==m_tapeB.begin()){
                m_tapeB.push_front('|');
                m_headB = m_tapeB.begin();
            }
            else{
                m_headB = std::prev(m_headB);
            }
        }

        // Change current state value
        presentState = m_states[transitionIdx].nextState;
    }
}

void TuringMachineMultiply::reset(std::deque<char> & tapeA, std::deque<char> & tapeB){
    m_tapeA = tapeA;
    m_headA = m_tapeA.begin();
    m_tapeB = tapeB;
    m_headB = m_tapeB.begin();
    m_memory = std::deque<char>();
    m_memory.push_front('^');
    m_memory.push_back('|');
    presentState = e_states::init;
}

// int main(){
//     std::deque<char> tapeA, tapeB;
//     tapeA.push_back('^');
//     tapeA.push_back('1');
//     tapeA.push_back('0');
//     tapeA.push_back('1');
//     tapeA.push_back('1');
//     tapeA.push_back('1');
//     tapeA.push_back('0');
//     tapeA.push_back('1');
//     tapeA.push_back('0');
//     tapeA.push_back('1');
//     tapeA.push_back('1');
//     tapeA.push_back('0');
//     tapeA.push_back('1');
//     tapeA.push_back('|');

//     tapeB.push_back('^');
//     tapeB.push_back('1');
//     tapeB.push_back('1');
//     tapeB.push_back('1');
//     tapeB.push_back('0');
//     tapeB.push_back('0');
//     tapeB.push_back('1');
//     tapeB.push_back('|');
//     TuringMachineMultiply tm(tapeA,tapeB);
//     for(auto i:tapeA){
//         std::cout << i;
//     }
//     std::cout << '*';
//     for(auto i:tapeB){
//         std::cout << i;
//     }
//     std::cout << '\n';
//     tm.simulate();
//     for(auto i:tapeB){
//         std::cout << i;
//     }
//     std::cout << '\n';
// }