/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "game/Board.h"
#include "synths/SampleSynth/SampleSynth.h"
#include "mapper/MidiMapper.h"

//==============================================================================
/**
*/
class GameOfLifeAudioProcessor  : public juce::AudioProcessor, juce::Timer
{
public:
    //==============================================================================
    GameOfLifeAudioProcessor();
    ~GameOfLifeAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void timerCallback() override;
    bool flag; // to do next step and update gui
private:
    Board gameOfLife;
    SampleSynth kickSynth;
    SampleSynth hhSynth;
    
    juce::Atomic<int> _bpm;
    juce::Atomic<double> _ppq;
    juce::Atomic<int> _current16thStep; // value between 0 and 15 (for one bar in 4 4 time)
    const int stepsPerBeat = 4;
    const int numSteps = 16;
    
    MidiMapper midiMapper;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GameOfLifeAudioProcessor)
};
