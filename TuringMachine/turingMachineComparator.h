#include <iostream>
#include <deque>
#include <vector>

class TuringMachineComparator{
private:

    std::string val; 
    enum e_stateControl{Normal,Start,End};
    enum e_direction{Neutral,Left,Right};
    enum e_states{
        init,
        getNonZero,
        lengthCheck,
        lengthCheckPA,
        lengthCheckPB,
        gte,
        lt,
        toFrontGTE,
        toFrontLT,
        hlt,
        
    };
    struct s_state{
        e_stateControl stateType;        // Normal state, start state or end state
        e_direction moveDirection[2];  // Movement of tapes after execution of the state
        char ruleInput[2];                  // Data for which the transition will occour
        char ruleOutput[2];                 // Emitted data.
        e_states currentState;       // Current state, in which we are.
        e_states nextState;          // next state, in which we will go after execution.
    };

    void p_process();
    void p_moveTape(std::deque<char>::iterator &, e_direction);
    int p_findStateTransition(char, char);
    void p_emmit(int);


    std::vector<s_state> m_states;

    e_states presentState;

    std::deque<char>::iterator m_headA;
    std::deque<char>::iterator m_headB;
    std::deque<char> & m_tapeA;
    std::deque<char> & m_tapeB;

public:

    TuringMachineComparator(std::deque<char> &, std::deque<char> &);
    
    void simulate();

    void reset(std::deque<char> &, std::deque<char> &);
};