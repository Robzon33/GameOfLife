/*
  ==============================================================================

    Board.cpp
    Created: 28 Nov 2023 9:23:55pm
    Author:  Tobias Koelker

  ==============================================================================
*/

#include "Board.h"

Board::Board(int size)
{
    cells.resize(size);
    
    for (int i = 0; i < size; ++i)
    {
        cells[i].resize(size);
    }
    
    this->generateRandom();
}

Board::~Board()
{
    
}

void Board::generateRandom()
{
    int boardSize = (int) cells.size();

    // Seed the random number generator with the current time
    juce::Random random(static_cast<int>(juce::Time::currentTimeMillis()));

    // Iterate over each cell in the board
    for (int x = 0; x < boardSize; ++x)
    {
        for (int y = 0; y < boardSize; ++y)
        {
            // Generate a random number between 0 and 1
            double randomValue = random.nextDouble();

            // Set the cell to be alive if the random value is below a certain threshold
            if (randomValue < 0.5) // Adjust the threshold as needed for desired density
            {
                cells[x][y].setState(State::Alive);
            }
            else
            {
                cells[x][y].setState(State::Dead);
            }
        }
    }
}

void Board::doNextStep()
{
    int boardSize = (int) cells.size();

    // Create a new vector to store the next state of each cell
    std::vector<std::vector<State>> nextState(boardSize, std::vector<State>(boardSize, State::Dead));

    // Iterate over each cell in the board
    for (int x = 0; x < boardSize; ++x)
    {
        for (int y = 0; y < boardSize; ++y)
        {
            // Get the number of alive neighbors for the current cell
            int numAliveNeighbors = getNumberOfNeighbours(x, y);
            
            bool cellIsAlive = false;
            if (cells[x][y].getState() == State::Alive)
            {
                cellIsAlive = true;
            }
            
            // Apply the rules
            bool isAliveAfterStep = settings.getNeighbourAffect(cellIsAlive, numAliveNeighbors);
            
            if (isAliveAfterStep)
            {
                nextState[x][y] = State::Alive;
            }
            else
            {
                nextState[x][y] = State::Dead;
            }
        }
    }

    // Update the states of cells based on the next state
    for (int x = 0; x < boardSize; ++x)
    {
        for (int y = 0; y < boardSize; ++y)
        {
            cells[x][y].setState(nextState[x][y]);
        }
    }
    
    this->sendChangeMessage();
}

int Board::getSize()
{
    return (int) cells.size();
}

State Board::getCellState(int x, int y)
{
    return cells[x][y].getState();
}

void Board::changeState(int x, int y)
{
    if (cells[x][y].getState() == State::Alive)
    {
        cells[x][y].setState(State::Dead);
    }
    else
    {
        cells[x][y].setState(State::Alive);
    }
}

int Board::getNumberOfNeighbours(int x, int y)
{
    int numNeighbours = 0;
    int boardSize = (int) cells.size(); // Assuming the board is square

    // Define offsets for neighboring cells
    int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    // Check each neighboring cell
    for (int i = 0; i < 8; ++i)
    {
        int nx = 0, ny = 0;
        
        if (settings.getTopology() == Topology::Boarders)
        {
            nx = x + dx[i];
            ny = y + dy[i];
        }
        else if (settings.getTopology() == Topology::Torus)
        {
            nx = (x + dx[i] + boardSize) % boardSize;
            ny = (y + dy[i] + boardSize) % boardSize;
        }

        // Check if the neighboring cell is within bounds
        if (nx >= 0 && nx < boardSize && ny >= 0 && ny < boardSize)
        {
            // Increment the number of neighbors if the cell is alive
            if (cells[nx][ny].getState() == State::Alive)
            {
                numNeighbours++;
            }
        }
    }

    return numNeighbours;
}
