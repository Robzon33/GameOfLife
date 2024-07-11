/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "game/Board.h"
#include "ui/GameComponent.h"

//==============================================================================
/**
*/
class GameOfLifeAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        public juce::Button::Listener,
                                        public juce::ChangeListener
{
public:
    GameOfLifeAudioProcessorEditor (GameOfLifeAudioProcessor&, Board&);
    ~GameOfLifeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void buttonClicked(juce::Button* button) override;
    
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GameOfLifeAudioProcessor& audioProcessor;
    
    GameComponent gameComp;
    Board& gameOfLife;
    
    juce::TextButton nextStepButton;
    juce::TextButton clearButton;
    juce::TextButton generateRandomButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameOfLifeAudioProcessorEditor)
};
