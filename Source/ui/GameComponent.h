/*
  ==============================================================================

    GameComponent.h
    Created: 28 Nov 2023 9:25:49pm
    Author:  Tobias Koelker

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../game/Board.h"
#include "../mapper/MidiMapper.h"

//==============================================================================
/*
*/
class GameComponent  : public juce::Component, public juce::Timer
{
public:
    GameComponent(Board&, MidiMapper&);
    ~GameComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void mouseDown(const juce::MouseEvent& event) override;
    void timerCallback() override;
private:
    Board& gameOfLife;
    MidiMapper& midiMapper;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameComponent)
};
