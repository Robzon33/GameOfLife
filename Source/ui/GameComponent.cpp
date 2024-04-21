/*
  ==============================================================================

    GameComponent.cpp
    Created: 28 Nov 2023 9:25:49pm
    Author:  Tobias Koelker

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GameComponent.h"

//==============================================================================
GameComponent::GameComponent(Board& b)
    : gameOfLife(b)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

GameComponent::~GameComponent()
{
}

void GameComponent::paint(juce::Graphics& g)
{
    // Clear the background
    g.fillAll(juce::Colours::white);

    // Get the size of the board
    int boardSize = gameOfLife.getSize();

    // Calculate the size of each cell
    int cellSize = std::min(getWidth() / boardSize, getHeight() / boardSize);
    
    // Calculate the colour
    static auto hue = 0.f;

    // Iterate over each cell in the board
    for (int x = 0; x < boardSize; ++x)
    {
        for (int y = 0; y < boardSize; ++y)
        {
            // Calculate the position of the cell
            int xPos = x * cellSize;
            int yPos = y * cellSize;

            g.setColour(juce::Colours::black);
            g.fillRect(xPos, yPos, cellSize, cellSize);
            
            juce::Colour colour = juce::Colour(255, 0, 0).withRotatedHue(hue);
            g.setColour(colour);
            g.drawRect(xPos, yPos, cellSize, cellSize, 1);

            // Check if the cell is alive
            if (gameOfLife.getCellState(x, y) == State::Alive)
            {
                // Draw a circle for the alive cell
                g.fillRect(xPos + 2, yPos + 2, cellSize - 4, cellSize - 4);
            }
        }
    }
    
    hue += .05f;
    if (hue > 1.f)
    {
        hue -= 1.f;
    }
}

void GameComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void GameComponent::mouseDown(const juce::MouseEvent& event)
{
    // Get the size of the board
    int boardSize = gameOfLife.getSize();

    // Calculate the size of each cell
    int cellSize = std::min(getWidth() / boardSize, getHeight() / boardSize);

    // Calculate the cell position based on the mouse click position
    int cellX = event.x / cellSize;
    int cellY = event.y / cellSize;

    // Ensure the calculated cell position is within the bounds of the board
    if (cellX >= 0 && cellX < boardSize && cellY >= 0 && cellY < boardSize)
    {
        gameOfLife.changeState(cellX, cellY);
        
        repaint(); // Repaint the component to update the display
    }
}
