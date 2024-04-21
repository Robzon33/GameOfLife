/*
  ==============================================================================

    Cell.h
    Created: 28 Nov 2023 9:24:40pm
    Author:  Tobias Koelker

  ==============================================================================
*/

#pragma once

enum class State { Dead, Alive};

class Cell
{
public:
    Cell();
    ~Cell();
    
    State getState();
    void setState(State newState);
private:
    State state;
};
