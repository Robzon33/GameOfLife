/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GameOfLifeAudioProcessorEditor::GameOfLifeAudioProcessorEditor (GameOfLifeAudioProcessor& p, Board& b)
    : AudioProcessorEditor (&p), audioProcessor (p), gameComp(b), gameOfLife(b)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    this->addAndMakeVisible(gameComp);
    
    addAndMakeVisible(nextStepButton);
    nextStepButton.setButtonText("Next Step");
    nextStepButton.addListener(this);
}

GameOfLifeAudioProcessorEditor::~GameOfLifeAudioProcessorEditor()
{
}

//==============================================================================
void GameOfLifeAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void GameOfLifeAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    auto b = this->getLocalBounds();
    
    gameComp.setBounds(b.removeFromRight(300));
    nextStepButton.setBounds(b.removeFromTop(20).reduced(5));
}

void GameOfLifeAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &nextStepButton)
    {
        gameOfLife.doNextStep(); // Call the method to perform the next step
        repaint(); // Update the display after the step
        audioProcessor.flag = true;
    }
}
