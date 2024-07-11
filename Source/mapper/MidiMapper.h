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
        
        //this->initDiatonicMapping();
        this->initChromaticMapping();
        this->printMap();
    };
    
    ~MidiMapper() {};
    
    void addMidiMessagesToBuffer(juce::MidiBuffer& midiMessages)
    {
        int counter = 0;
        for (int y = 0; y < board.getSize(); y++)
        {
            for (int x = board.getSize() - 1; x >= 0; x--) // iterating from right to left
            {
                if (board.getCellState(x, y) == State::Alive)
                {
                    int noteNumber = map[x][y];
                    juce::MidiMessage noteOn = juce::MidiMessage::noteOn(2, noteNumber, 0.2f);
                    midiMessages.addEvent(noteOn, 0);
                    counter++;
                    break; // go to next row
                }
            }
        }
        DBG("Number of midi messages added: " << counter);
    };
    
    void initDiatonicMapping()
    {
        int rootNote = 24; // MIDI note number for C0
        int scale[] = {0, 2, 4, 5, 7, 9, 11}; // Major scale intervals
        int scaleLength = sizeof(scale) / sizeof(scale[0]);
        
        int boardSize = board.getSize();
        
        for (int y = 0; y < boardSize; ++y)
        {
            for (int x = 0; x < boardSize; ++x)
            {
                int scaleOffset = y * 2; // Every new row starts from the next scale degree
                int note = rootNote + scale[(x + scaleOffset) % scaleLength] + 12 * ((x + scaleOffset) / scaleLength);
                map[x][y] = note;
            }
        }
    };
    
    void initChromaticMapping()
    {
        int rootNote = 24; // MIDI note number for C0
        
        int boardSize = board.getSize();
        
        for (int y = 0; y < boardSize; ++y)
        {
            int note = rootNote + (y * 3);
            for (int x = 0; x < boardSize; ++x)
            {
                map[x][y] = note + x;
            }
        }
    };
private:
    void printMap() const
    {
        for (int y = 0; y < map.size(); ++y)
        {
            juce::String rowString = "Row " + juce::String(y) + ": ";
            for (int x = 0; x < map[y].size(); ++x)
            {
                rowString += "(" + juce::String(x) + "," + juce::String(y) + ":" + juce::String(map[x][y]) + ") ";
            }
            DBG(rowString.trimEnd());
        }
    };
    
    Board& board;
    std::vector<std::vector<int>> map; // to store the midi note numbers
};
