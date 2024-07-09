/*
  ==============================================================================

    MidiMapper.h
    Created: 9 Jul 2024 11:43:51am
    Author:  Tobias Koelker

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../game/Board.h"

class MidiMapper
{
public:
    MidiMapper(Board& b)
    : board(b)
    {
        map.resize(board.getSize());
        
        for (int i = 0; i < board.getSize(); i++)
        {
            map[i].resize(board.getSize());
        }
        
        this->initMapping();
        this->printMap();
    };
    
    ~MidiMapper() {};
private:
    void initMapping()
    {
        int rootNote = 36; // MIDI note number for C1
        int scale[] = {0, 2, 4, 5, 7, 9, 11}; // Major scale intervals
        int scaleLength = sizeof(scale) / sizeof(scale[0]);
        
        int boardSize = board.getSize();
        
        for (int row = 0; row < boardSize; ++row)
        {
            for (int col = 0; col < boardSize; ++col)
            {
                int scaleOffset = row * 2; // Every new row starts from the next scale degree
                int note = rootNote + scale[(col + scaleOffset) % scaleLength] + 12 * ((col + scaleOffset) / scaleLength);
                map[row][col] = note;
            }
        }
    };
    
    void printMap() const
    {
        for (int row = 0; row < map.size(); ++row)
        {
            juce::String rowString = "Row " + juce::String(row) + ": ";
            for (int col = 0; col < map[row].size(); ++col)
            {
                rowString += "(" + juce::String(row) + "," + juce::String(col) + ":" + juce::String(map[row][col]) + ") ";
            }
            DBG(rowString.trimEnd());
        }
    };
    
    Board& board;
    std::vector<std::vector<int>> map; // to store the midi note numbers
};
