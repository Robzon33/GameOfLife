/*
  ==============================================================================

    SimpleReverb.h
    Created: 11 Jul 2024 8:08:13pm
    Author:  Tobias Koelker

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SimpleReverb
{
public:
    SimpleReverb()
    {
        params.roomSize = 0.8f;
        params.damping = 0.4f;
        params.wetLevel = 0.7f;
        params.dryLevel = 0.3f;
        params.width = 0.4f;
        params.freezeMode = 0.4f; // > 0.5 causes continous feedback loop
        
        reverb.setParameters(params);
    };
    
    ~SimpleReverb() {};
    
    void setCurrentPlaybackSampleRate(double sampleRate)
    {
        reverb.setSampleRate(sampleRate);
    };
    
    void process(juce::AudioBuffer<float>& buffer, int numSamples)
    {
        reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), numSamples);
    };
private:
    juce::Reverb reverb;
    juce::Reverb::Parameters params;
};
