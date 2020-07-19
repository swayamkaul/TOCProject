#include "turingMachineAdd.h"



TuringMachineAdd::TuringMachineAdd(std::deque<char> & tapeA, std::deque<char> & tapeB):m_tapeA(tapeA),m_tapeB(tapeB){
    presentState = e_states::init;
    m_headA = m_tapeA.begin();
    m_headB = m_tapeB.begin();

    // Initial state transition
    m_states.push_back({e_stateControl::Start,{e_direction::Left,e_direction::Left},{'^','^'},{'n','n'},e_states::init,e_states::goToEnd});

    // Transitions for reaching the end of binary number
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Left},{'0','0'},{'n','n'},e_states::goToEnd,e_states::goToEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Left},{'0','1'},{'n','n'},e_states::goToEnd,e_states::goToEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Left},{'1','1'},{'n','n'},e_states::goToEnd,e_states::goToEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Left},{'1','0'},{'n','n'},e_states::goToEnd,e_states::goToEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Left},{'|','0'},{'n','n'},e_states::goToEnd,e_states::goToEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Left},{'|','1'},{'n','n'},e_states::goToEnd,e_states::goToEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Neutral},{'0','|'},{'n','n'},e_states::goToEnd,e_states::goToEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Neutral},{'1','|'},{'n','n'},e_states::goToEnd,e_states::goToEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Right,e_direction::Right},{'|','|'},{'n','n'},e_states::goToEnd,e_states::st_0carry});

    // Transitions for computation of of addition
    // carryState:0
    m_states.push_back({e_stateControl::Normal,{e_direction::Right,e_direction::Right},{'0','0'},{'n','0'},e_states::st_0carry,e_states::st_0carry});
    m_states.push_back({e_stateControl::Normal,{e_direction::Right,e_direction::Right},{'0','1'},{'n','1'},e_states::st_0carry,e_states::st_0carry});
    m_states.push_back({e_stateControl::Normal,{e_direction::Right,e_direction::Right},{'1','0'},{'n','1'},e_states::st_0carry,e_states::st_0carry});
    m_states.push_back({e_stateControl::Normal,{e_direction::Right,e_direction::Right},{'1','1'},{'n','0'},e_states::st_0carry,e_states::st_1carry});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'^','0'},{'n','0'},e_states::st_0carry,e_states::st_0carry});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'^','1'},{'n','1'},e_states::st_0carry,e_states::st_0carry});
    m_states.push_back({e_stateControl::Normal,{e_direction::Right,e_direction::Right},{'0','^'},{'n','0'},e_states::st_0carry,e_states::st_0carry});
    m_states.push_back({e_stateControl::Normal,{e_direction::Right,e_direction::Right},{'1','^'},{'n','1'},e_states::st_0carry,e_states::st_0carry});
    m_states.push_back({e_stateControl::End,{e_direction::Neutral,e_direction::Neutral},{'^','^'},{'n','n'},e_states::st_0carry,e_states::hlt});

    // carryState:1
    m_states.push_back({e_stateControl::Normal,{e_direction::Right,e_direction::Right},{'0','0'},{'n','1'},e_states::st_1carry,e_states::st_0carry});
    m_states.push_back({e_stateControl::Normal,{e_direction::Right,e_direction::Right},{'0','1'},{'n','0'},e_states::st_1carry,e_states::st_1carry});
    m_states.push_back({e_stateControl::Normal,{e_direction::Right,e_direction::Right},{'1','0'},{'n','0'},e_states::st_1carry,e_states::st_1carry});
    m_states.push_back({e_stateControl::Normal,{e_direction::Right,e_direction::Right},{'1','1'},{'n','1'},e_states::st_1carry,e_states::st_1carry});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'^','0'},{'n','1'},e_states::st_1carry,e_states::st_0carry});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'^','1'},{'n','0'},e_states::st_1carry,e_states::st_1carry});
    m_states.push_back({e_stateControl::Normal,{e_direction::Right,e_direction::Right},{'0','^'},{'n','1'},e_states::st_1carry,e_states::st_0carry});
    m_states.push_back({e_stateControl::Normal,{e_direction::Right,e_direction::Right},{'1','^'},{'n','0'},e_states::st_1carry,e_states::st_1carry});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'^','^'},{'n','1'},e_states::st_1carry,e_states::appendBegin});
    m_states.push_back({e_stateControl::End,{e_direction::Neutral,e_direction::Neutral},{'^','^'},{'n','^'},e_states::appendBegin,e_states::hlt});
}

int TuringMachineAdd::p_findStateTransition(char sA, char sB){
    for(int i = 0; i < m_states.size(); i++){
        if((m_states[i].currentState == presentState) && (m_states[i].ruleInput[0] == sA || m_states[i].ruleInput[0] == '*') && (m_states[i].ruleInput[1]==sB || m_states[i].ruleInput[1]=='*')){
            return i;
        }
    }
    throw "Error in add";
}

void TuringMachineAdd::p_emmit(int transitionId){
    if(m_states[transitionId].ruleOutput[0]!='n'){
        *m_headA = m_states[transitionId].ruleOutput[0];
    }
    if(m_states[transitionId].ruleOutput[1]!='n'){
        *m_headB = m_states[transitionId].ruleOutput[1];
    }
}

void TuringMachineAdd::simulate(){
    while(presentState!=e_states::hlt){
        // Get input symbols from tape
        char sA = *m_headA;
        char sB = *m_headB;
        
        // if(presentState!=goToEnd){
        //     std::cerr << 'p';
        // }

        // Find transition
        int transitionIdx = p_findStateTransition(sA,sB);

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
                m_tapeA.push_front('^');
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
                m_tapeB.push_front('^');
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

void TuringMachineAdd::reset(std::deque<char> & tapeA, std::deque<char> & tapeB){
    m_tapeA = tapeA;
    m_headA = m_tapeA.begin();
    m_tapeB = tapeB;
    m_headB = m_tapeB.begin();
    presentState = e_states::init;
}


// int main(){
//     std::deque<char> tapeA;
//     std::deque<char> tapeB;
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
//     tapeB.push_back('0');
//     tapeB.push_back('1');
//     tapeB.push_back('1');
//     tapeB.push_back('1');
//     tapeB.push_back('0');
//     tapeB.push_back('1');
//     tapeB.push_back('0');
//     tapeB.push_back('1');
//     tapeB.push_back('1');
//     tapeB.push_back('0');
//     tapeB.push_back('1');
//     tapeB.push_back('0');
//     tapeB.push_back('0');
//     tapeB.push_back('0');
//     tapeB.push_back('|');

//     std::cout << "Input: \n";
//     for(auto i:tapeA){
//         std::cout << char(i);
//     }
//     std::cout << '\n';
//     for(auto i:tapeB){
//         std::cout << char(i);
//     }
//     std::cout << '\n';
//     TuringMachineAdd tm(tapeA,tapeB);
//     tm.simulate();
//     std::cout << "Output: \n";
//     for(auto i:tapeA){
//         std::cout << char(i);
//     }
//     std::cout << '\n';
//     for(auto i:tapeB){
//         std::cout << char(i);
//     }
//     std::cout << '\n';
// }