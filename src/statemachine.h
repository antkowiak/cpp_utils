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
#include <map>
#include <queue>

template <typename StateType, typename EventType>
class statemachine
{
public:
    // define a transition to be a pair of a FromState and an Event
    typedef std::pair<StateType, EventType> transition_t;

    // callback to call when entering a particular state
    typedef std::function<void(const StateType &, const EventType &, const StateType &)> state_function_t;

    // callback to call when a specific event causes a transition from one specific state to another specific state
    typedef std::function<void(const StateType &, const EventType &, const StateType &)> transition_function_t;

    // callback to call when there is an unexpected event in the current state
    typedef std::function<void(const StateType &, const EventType &)> unhandled_event_callback_t;

protected:
    // the initial state for the state machine
    const StateType initial_state;

    // the current state of the state machine
    StateType current_state;

    // map of transitions to what the next state should be
    std::map<transition_t, StateType> next_state_map;

    // queue of the names of transitions to process
    std::queue<EventType> event_queue;

    // map of transitions and what functions are called on those transitions
    std::map<transition_t, transition_function_t> transition_functions;

    // map of states and what functions are called in those states.
    std::map<StateType, state_function_t> state_functions;

    // callback when there is an unhandled event
    unhandled_event_callback_t unhandled_event_callback;

public:
    // constructor
    statemachine(const StateType &initialState)
        : initial_state(initialState), current_state(initialState)
    {
        add_state(initial_state);
    }

    // set a callback for when an unexpected event occurs in the current state
    void set_unhandled_event_callback(const unhandled_event_callback_t &cb)
    {
        unhandled_event_callback = cb;
    }

    // add a state to the state machine
    void add_state(const StateType &state,
                   const state_function_t &func = state_function_t())
    {
        state_functions[state] = func;
    }

    // add a transition to the state mcahine
    void add_transition(const StateType &fromState,
                        const EventType &event,
                        const StateType &toState,
                        const transition_function_t &func = transition_function_t())
    {
        const transition_t trans(fromState, event);
        transition_functions[trans] = func;
        next_state_map[trans] = toState;
    }

    // push an event into the state machine
    void push_event(const EventType &transition)
    {
        event_queue.push(transition);
    }

    // run all the pending events in the state machine
    size_t run()
    {
        long statesRun = 0;

        while (!event_queue.empty())
        {
            ++statesRun;
            run_one_event();
        }

        // return the number of states that were run
        return statesRun;
    }

    // run one pending event in the state machine
    bool run_one_event()
    {
        // if there are no events pending, return immediately
        if (event_queue.empty())
        {
            return false;
        }

        // pick the front of the queue of pending events
        const EventType event = event_queue.front();

        // the transition consistes of the name of the current state and
        // the name of the transition to the next state
        const transition_t transitionToFind(current_state, event);

        // return immediately if the transition doesn't exist, and discard event.
        if (transition_functions.find(transitionToFind) ==
            transition_functions.end())
        {
            if (unhandled_event_callback)
                unhandled_event_callback(current_state, event);

            event_queue.pop();
            return false;
        }

        // return immediately if the state transition doesn't exist,
        // and discard the event
        if (next_state_map.find(transitionToFind) == next_state_map.end())
        {
            if (unhandled_event_callback)
                unhandled_event_callback(current_state, event);

            event_queue.pop();
            return false;
        }

        // after this event, the previous state will be what the
        // current state is now
        const StateType prevState = current_state;

        // grab the next state that the machine will transition to
        const StateType nextState = next_state_map[transitionToFind];

        // call the transition function
        const transition_function_t transFunc = transition_functions[transitionToFind];
        if (transFunc)
            transFunc(prevState, event, nextState);

        // update the current state to be the next state in the state machine
        current_state = nextState;

        // remove the event from the queue that we are processing
        event_queue.pop();

        // call the current state function
        const state_function_t stateFunc = state_functions[current_state];
        if (stateFunc)
            stateFunc(prevState, event, nextState);

        return true;
    }

    // returns the current state
    StateType get_current_state() const
    {
        return current_state;
    }

    // reset the state machine by returning to the initial state
    void reset()
    {
        // return to the initial state
        current_state = initial_state;

        // Clear the event queue
        event_queue = std::queue<EventType>();
    }

}; // class statemachine
