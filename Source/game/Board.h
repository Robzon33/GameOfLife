/*
  ==============================================================================

    Board.h
    Created: 28 Nov 2023 9:23:55pm
    Author:  Tobias Koelker

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Cell.h"
#include "Settings.h"

class Board
{
public:
    Board(int size);
    ~Board();
    
    void generateRandom();
    void doNextStep();
    int getSize();
    State getCellState(int x, int y);
    void changeState(int x, int y);
    void clear();
private:
    int getNumberOfNeighbours(int x, int y);
    std::vector<std::vector<Cell>> cells;
    Settings settings;
};
