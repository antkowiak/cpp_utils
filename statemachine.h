#pragma once

//
// statemachine.h - Utility to create a simple state machine.
//  Functions can be called on transitions and when states are entered.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//
// 2016
//

#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <utility>

typedef std::string TStateName;

typedef std::string TTransitionName;

typedef std::pair<TStateName, TTransitionName> TTransition;

typedef std::function<void(const TStateName &, const TTransitionName &,
                           const TStateName &)>
    TStateFunction;

typedef std::function<void(const TStateName &, const TTransitionName &,
                           const TStateName &)>
    TTransitionFunction;

/**
 *
 * @class Statemachine.h
 * Simple implementation of a state machine design pattern, all in one header.
 *
 * @brief State machine design pattern, all in one header file.
 *
 **/
class statemachine
{
public:
    // Returns the name of a default initial state name to use, if one is
    // not specified upon construction.
    static const TStateName get_default_initial_state()
    {
        return TStateName("init");
    }

    // Default constructor.  Will use default initial state.
    statemachine()
        : statemachine(get_default_initial_state())
    {
    }

    // Constructor.  Initial state will be initialState.
    statemachine(const TStateName &initialState)
        : m_initial_state(initialState), m_current_state(initialState)
    {
    }

    // Destructor
    ~statemachine()
    {
    }

    // Will print out information about the state machine.
    void print() const
    {
        std::cout << "m_initial_state = " << m_initial_state << std::endl;
        std::cout << "m_current_state = " << m_current_state << std::endl;

        if (!m_transition_queue.empty())
        {
            std::cout << "next transition: " << m_transition_queue.front()
                      << std::endl;
        }

        for (auto &iter : m_next_states)
        {
            std::cout << "transition: {" << iter.first.first << ", "
                      << iter.first.second << "} -> " << iter.second << std::endl;
        }
    }

    // Add a state to the state machine.
    void add_state(const TStateName &stateName, const TStateFunction &func)
    {
        m_state_functions[stateName] = func;
    }

    // Add a transition to the state mcahine.
    void add_transition(const TStateName &prevState,
                        const TTransitionName &transition,
                        const TStateName &nextState,
                        const TTransitionFunction &func)
    {
        const TTransition trans(prevState, transition);
        m_transition_functions[trans] = func;
        m_next_states[trans] = nextState;
    }

    // Push an event/transition into the state machine.
    void push_transition(const TTransitionName &transition)
    {
        m_transition_queue.push(transition);
    }

    // Run all the pending transitions in the state machine.
    long run()
    {
        long statesRun = 0;

        while (!m_transition_queue.empty())
        {
            ++statesRun;
            run_once_state();
        }

        // Return the number of states that were run.
        return statesRun;
    }

    // Run one pending transition in the state machine.
    bool run_once_state()
    {
        // If there are no transitions pending, return immediately.
        if (m_transition_queue.empty())
        {
            return false;
        }

        // Pick the front of the queue of pending transitions.
        const TTransitionName transition = m_transition_queue.front();

        // The transition consistes of the name of the current state and
        // the name of the transition to the next state.
        const TTransition transitionToFind(m_current_state, transition);

        // Return immediately if the transition doesn't exist.
        if (m_transition_functions.find(transitionToFind) ==
            m_transition_functions.end())
        {
            return false;
        }

        // Return immediately if the state transition doesn't exist.
        if (m_next_states.find(transitionToFind) == m_next_states.end())
        {
            return false;
        }

        // After this transition, the previous state will be what the
        // current state is now.
        const TStateName prevState = m_current_state;

        // Grab the next state that the machine will transition to.
        const TStateName nextState = m_next_states[transitionToFind];

        // Grab the transition function to call.
        const TTransitionFunction func = m_transition_functions[transitionToFind];

        // Call the transition function.
        func(prevState, transition, nextState);

        // Update the current state to be the next state in the state
        // machine.
        m_current_state = nextState;

        // Remove the transition from the queue that we are processing.
        m_transition_queue.pop();

        // Call the current state function
        m_state_functions[m_current_state](prevState, transition, nextState);

        return true;
    }

    // Reset the state machine by returning to the initial state.
    void reset()
    {
        // Return to the initial state.
        m_current_state = m_initial_state;

        // Clear the transition queue.
        m_transition_queue = std::queue<TTransitionName>();
    }

protected:
    // The starting state for the state machine.
    const TStateName m_initial_state;

    // The current state of the state machine.
    TStateName m_current_state;

    // Map of transitions to what the next state should be.
    std::map<TTransition, TStateName> m_next_states;

    // Queue of the names of transitions/events to process.
    std::queue<TTransitionName> m_transition_queue;

    // Map of transitions and what functions are called on those
    // transitions.
    std::map<TTransition, TTransitionFunction> m_transition_functions;

    // Map of states and what functions are called in those states.
    std::map<TStateName, TStateFunction> m_state_functions;
}; // class statemachine
