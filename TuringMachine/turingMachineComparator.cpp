#include "turingMachineComparator.h"

TuringMachineComparator::TuringMachineComparator(std::deque<char> & tapeA, std::deque<char> & tapeB):m_tapeA(tapeA),m_tapeB(tapeB){
    presentState = e_states::init;
    m_headA = m_tapeA.begin();
    m_headB = m_tapeB.begin();

    // Initial state transition
    m_states.push_back({e_stateControl::Start,{e_direction::Left,e_direction::Left},{'^','^'},{'n','n'},e_states::init,e_states::getNonZero});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Left},{'0','0'},{'n','n'},e_states::getNonZero,e_states::getNonZero});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Neutral},{'0','1'},{'n','n'},e_states::getNonZero,e_states::getNonZero});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Neutral},{'0','|'},{'n','n'},e_states::getNonZero,e_states::getNonZero});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Left},{'1','0'},{'n','n'},e_states::getNonZero,e_states::getNonZero});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Left},{'|','0'},{'n','n'},e_states::getNonZero,e_states::getNonZero});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'1','1'},{'n','n'},e_states::getNonZero,e_states::toEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'1','|'},{'n','n'},e_states::getNonZero,e_states::toEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'|','1'},{'n','n'},e_states::getNonZero,e_states::toEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'|','|'},{'n','n'},e_states::getNonZero,e_states::toEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'|','0'},{'n','n'},e_states::toEnd,e_states::lt});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'|','1'},{'n','n'},e_states::toEnd,e_states::lt});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'0','|'},{'n','n'},e_states::toEnd,e_states::gt});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'1','|'},{'n','n'},e_states::toEnd,e_states::gt});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Left},{'0','0'},{'n','n'},e_states::toEnd,e_states::toEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Left},{'1','1'},{'n','n'},e_states::toEnd,e_states::toEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Left},{'1','0'},{'n','n'},e_states::toEnd,e_states::toEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Left},{'0','1'},{'n','n'},e_states::toEnd,e_states::toEnd});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'|','|'},{'n','n'},e_states::toEnd,e_states::token});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Left},{'0','0'},{'n','n'},e_states::token,e_states::token});
    m_states.push_back({e_stateControl::Normal,{e_direction::Left,e_direction::Left},{'1','1'},{'n','n'},e_states::token,e_states::token});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'1','|'},{'n','n'},e_states::token,e_states::gt});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'0','|'},{'n','n'},e_states::token,e_states::gt});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'|','1'},{'n','n'},e_states::token,e_states::lt});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'|','0'},{'n','n'},e_states::token,e_states::lt});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'0','1'},{'n','n'},e_states::token,e_states::lt});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'1','0'},{'n','n'},e_states::token,e_states::gt});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Neutral},{'|','|'},{'n','n'},e_states::token,e_states::eq});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'*','*'},{'n','n'},e_states::lt,e_states::goFrontl});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'*','*'},{'n','n'},e_states::eq,e_states::goFronte});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'*','*'},{'n','n'},e_states::gt,e_states::goFrontg});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'*','0'},{'n','n'},e_states::goFronte,e_states::goFronte});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'*','1'},{'n','n'},e_states::goFronte,e_states::goFronte});
    m_states.push_back({e_stateControl::End,{e_direction::Neutral,e_direction::Neutral},{'*','^'},{'n','1'},e_states::goFronte,e_states::hlt});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'*','0'},{'n','n'},e_states::goFrontl,e_states::goFrontl});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'*','1'},{'n','n'},e_states::goFrontl,e_states::goFrontl});
    m_states.push_back({e_stateControl::End,{e_direction::Neutral,e_direction::Neutral},{'*','^'},{'n','0'},e_states::goFrontl,e_states::hlt});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'*','0'},{'n','n'},e_states::goFrontg,e_states::goFrontg});
    m_states.push_back({e_stateControl::Normal,{e_direction::Neutral,e_direction::Right},{'*','1'},{'n','n'},e_states::goFrontg,e_states::goFrontg});
    m_states.push_back({e_stateControl::End,{e_direction::Neutral,e_direction::Neutral},{'*','^'},{'n','1'},e_states::goFrontg,e_states::hlt});
}

int TuringMachineComparator::p_findStateTransition(char sA, char sB){
    for(int i = 0; i < m_states.size(); i++){
        if((m_states[i].currentState == presentState) && (m_states[i].ruleInput[0] == sA || m_states[i].ruleInput[0] == '*') && (m_states[i].ruleInput[1]==sB || m_states[i].ruleInput[1]=='*')){
            return i;
        }
    }
    throw "Error";
}

void TuringMachineComparator::p_emmit(int transitionId){
    if(m_states[transitionId].ruleOutput[0]!='n'){
        *m_headA = m_states[transitionId].ruleOutput[0];
    }
    if(m_states[transitionId].ruleOutput[1]!='n'){
        *m_headB = m_states[transitionId].ruleOutput[1];
    }
}

void TuringMachineComparator::simulate(){
    while(presentState!=e_states::hlt){
        // Get input symbols from tape
        char sA = *m_headA;
        char sB = *m_headB;
        
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

void TuringMachineComparator::reset(std::deque<char> & tapeA, std::deque<char> & tapeB){
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
//     tapeB.push_back('0');
//     tapeB.push_back('1');
//     tapeB.push_back('1');
//     tapeB.push_back('0');
//     tapeB.push_back('1');
//     tapeB.push_back('|');
//     TuringMachineComparator tm(tapeA, tapeB);
//     tm.simulate();
//     std::cout << tapeB[0] << '\n';
// }