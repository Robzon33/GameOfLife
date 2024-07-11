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
    setSize (800, 600);
    this->setResizable(true, true);
    
    this->addAndMakeVisible(gameComp);
    
    addAndMakeVisible(nextStepButton);
    nextStepButton.setButtonText("Next Step");
    nextStepButton.addListener(this);
    
    addAndMakeVisible(clearButton);
    clearButton.setButtonText("Clear Board");
    clearButton.addListener(this);
    
    addAndMakeVisible(generateRandomButton);
    generateRandomButton.setButtonText("Fill random");
    generateRandomButton.addListener(this);
    
    gameOfLife.addChangeListener(this);
}

GameOfLifeAudioProcessorEditor::~GameOfLifeAudioProcessorEditor()
{
    gameOfLife.removeChangeListener(this);
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
    
    gameComp.setBounds(b.removeFromRight(b.getWidth() / 5 * 4));
    nextStepButton.setBounds(b.removeFromTop(40).reduced(5));
    clearButton.setBounds(b.removeFromTop(40).reduced(5));
    generateRandomButton.setBounds(b.removeFromTop(40).reduced(5));
}

void GameOfLifeAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &nextStepButton)
    {
        gameOfLife.doNextStep(); // Call the method to perform the next step
        repaint(); // Update the display after the step
        audioProcessor.flag = true;
    }
    if (button ==&clearButton)
    {
        gameOfLife.clear();
        repaint();
    }
    if (button == &generateRandomButton)
    {
        gameOfLife.generateRandom();
        repaint();
    }
}

void GameOfLifeAudioProcessorEditor::changeListenerCallback(juce::ChangeBroadcaster *source)
{
    this->repaint();
}
