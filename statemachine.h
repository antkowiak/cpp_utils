#pragma once

/**
 *
 * @file statemachine.h
 * Simple implementation of a state machine design pattern, all in one header.
 *
 * @brief State machine design pattern, all in one header file.
 *
 * @author Ryan Antkowiak (antkowiak@gmail.com)
 *
 * @note Uses some c++11 language features. Compile with -std=c++11
 *
 * @copyright Ryan Antkowiak 2016, All rights reserved.
 *
 **/

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

typedef std::function<void(const TStateName&,
    const TTransitionName&,
    const TStateName&)> TStateFunction;
typedef std::function<void(const TStateName&,
    const TTransitionName&,
    const TStateName&)> TTransitionFunction;
/**
 *
 * @class Statemachine.h
 * Simple implementation of a state machine design pattern, all in one header.
 *
 * @brief State machine design pattern, all in one header file.
 *
 **/
class Statemachine
{
public:

    /// Returns the name of a default initial state name to use, if one is
    // not specified upon construction.
    static const TStateName getDefaultInitialState()
    {
        return TStateName("init");
    }

    /// Default constructor.  Will use default initial state.
    Statemachine()
        : Statemachine(getDefaultInitialState())
    {
    }

    /// Constructor.  Initial state will be initialState.
    Statemachine(const TStateName& initialState)
        : m_initialState(initialState), m_currentState(initialState)
    {
    }

    /// Destructor
    ~Statemachine()
    {
    }

    /// Will print out information about the state machine.
    void print() const
    {
        std::cout << "m_initialState = " << m_initialState << std::endl;
        std::cout << "m_currentState = " << m_currentState << std::endl;

        if (!m_transitionQueue.empty())
        {
            std::cout << "next transition: " << m_transitionQueue.front() << std::endl;
        }

        for (auto& iter : m_nextStates)
        {
            std::cout << "transition: {" << iter.first.first << ", "
                << iter.first.second << "} -> " << iter.second
                << std::endl;
        }
    }

    /// Add a state to the state machine.
    void addState(const TStateName& stateName, const TStateFunction& func)
    {
        m_stateFunctions[stateName] = func;
    }

    /// Add a transition to the state mcahine.
    void addTransition(const TStateName& prevState,
        const TTransitionName& transition,
        const TStateName& nextState,
        const TTransitionFunction& func)
    {
        const TTransition trans(prevState, transition);
        m_transitionFunctions[trans] = func;
        m_nextStates[trans] = nextState;
    }

    /// Push an event/transition into the state machine.
    void pushTransition(const TTransitionName& transition)
    {
        m_transitionQueue.push(transition);
    }

    /// Run all the pending transitions in the state machine.
    long run()
    {
        long statesRun = 0;

        while (!m_transitionQueue.empty())
        {
            ++statesRun;
            runOneState();
        }

        // Return the number of states that were run.
        return statesRun;
    }

    /// Run one pending transition in the state machine.
    bool runOneState()
    {
        // If there are no transitions pending, return immediately.
        if (m_transitionQueue.empty())
        {
            return false;
        }

        // Pick the front of the queue of pending transitions.
        const TTransitionName transition = m_transitionQueue.front();

        // The transition consistes of the name of the current state and
        // the name of the transition to the next state.
        const TTransition transitionToFind(m_currentState, transition);

        // Return immediately if the transition doesn't exist.
        if (m_transitionFunctions.find(transitionToFind) == m_transitionFunctions.end())
        {
            return false;
        }

        // Return immediately if the state transition doesn't exist.
        if (m_nextStates.find(transitionToFind) == m_nextStates.end())
        {
            return false;
        }

        // After this transition, the previous state will be what the
        // current state is now.
        const TStateName prevState = m_currentState;

        // Grab the next state that the machine will transition to.
        const TStateName nextState = m_nextStates[transitionToFind];

        // Grab the transition function to call.
        const TTransitionFunction func = m_transitionFunctions[transitionToFind];

        // Call the transition function.
        func(prevState, transition, nextState);

        // Update the current state to be the next state in the state
        // machine.
        m_currentState = nextState;

        // Remove the transition from the queue that we are processing.
        m_transitionQueue.pop();

        // Call the current state function
        m_stateFunctions[m_currentState](prevState, transition, nextState);

        return true;
    }

    /// Reset the state machine by returning to the initial state.
    void reset()
    {
        // Return to the initial state.
        m_currentState = m_initialState;

        // Clear the transition queue.
        m_transitionQueue = std::queue<TTransitionName>();
    }

private:

    /// The starting state for the state machine.
    const TStateName m_initialState;

    /// The current state of the state machine.
    TStateName m_currentState;

    /// Map of transitions to what the next state should be.
    std::map<TTransition, TStateName> m_nextStates;

    /// Queue of the names of transitions/events to process.
    std::queue<TTransitionName> m_transitionQueue;

    /// Map of transitions and what functions are called on those
    // transitions.
    std::map<TTransition, TTransitionFunction> m_transitionFunctions;

    /// Map of states and what functions are called in those states.
    std::map<TStateName, TStateFunction> m_stateFunctions;
};

