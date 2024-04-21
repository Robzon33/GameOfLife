/*
  ==============================================================================

    Cell.cpp
    Created: 28 Nov 2023 9:24:40pm
    Author:  Tobias Koelker

  ==============================================================================
*/

#include "Cell.h"

Cell::Cell()
{
    state = State::Dead;
}

Cell::~Cell()
{
    
}

State Cell::getState()
{
    return state;
}

void Cell::setState(State newState)
{
    state = newState;
}
