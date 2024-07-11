/*
  ==============================================================================

    SinewaveSynth.h
    Created: 11 Jul 2024 4:12:53pm
    Author:  Tobias Koelker

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/*
 The synthesisers sound
 */
class SynthSound : public juce::SynthesiserSound
{
public:
    SynthSound() {};
    ~SynthSound() {};
    bool appliesToNote(int /*midiNoteNumber*/) override { return true; };
    bool appliesToChannel(int /*midiChannel*/) override { return true; };
};

/*
 The synthesisers voice
 */
class SynthVoice : public juce::SynthesiserVoice
{
public:
    SynthVoice() {};

    // Overriding SynthesiserVoice functions
    bool canPlaySound(juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<SynthSound*> (sound) != nullptr;
    };
    
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentAngle = 0.0;
        level = velocity * 0.15;
        tailOff = 0.0;
        
        auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        DBG("Midi Note started: " << midiNoteNumber);
        
        auto cyclesPerSample = cyclesPerSecond / getSampleRate();
        
        angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
    };
    
    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            if (tailOff == 0.0)
                tailOff = 1.0;
        }
        else
        {
            clearCurrentNote();
            angleDelta = 0.0;
        }
    };
    
    void pitchWheelMoved(int /*newValue*/) override {};
    
    void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override {};
    
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        auto numActiveVoices = 20;
        float normalizationFactor = (numActiveVoices > 0) ? 1.0f / numActiveVoices : 1.0f;
        
        if (angleDelta != 0.0)
        {
            if (tailOff > 0.0)
            {
                while (--numSamples >= 0)
                {
                    auto currentSample = (float) (std::sin (currentAngle) * level * tailOff * normalizationFactor);
                    
                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample (i, startSample, currentSample);
                    
                    currentAngle += angleDelta;
                    ++startSample;
                    
                    tailOff *= 0.99;
                    
                    if (tailOff <= 0.005)
                    {
                        clearCurrentNote();
                        
                        angleDelta = 0.0;
                        break;
                    }
                }
            }
            else
            {
                while (--numSamples >= 0)
                {
                    auto currentSample = (float) (std::sin (currentAngle) * level * normalizationFactor);
                    
                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample (i, startSample, currentSample);
                    
                    currentAngle += angleDelta;
                    ++startSample;
                }
            }
        }
    };
private:
    double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0;
};

/*
 The synthesiser class
 */
class SinewaveSynth : public juce::Synthesiser
{
public:
    SinewaveSynth()
    {
        this->clearVoices();
        
        auto numVoices = 20;
        for (auto i = 0; i < numVoices; ++i)
        {
            this->addVoice(new SynthVoice());
        }
        
        this->clearSounds();
        this->addSound(new SynthSound());
    };
    
    ~SinewaveSynth() {};
    
    int getNumActiveVoices() const
    {
        int activeVoices = 0;
        for (int i = 0; i < getNumVoices(); ++i)
        {
            if (auto* voice = dynamic_cast<SynthVoice*>(getVoice(i)))
            {
                if (voice->isVoiceActive())
                {
                    activeVoices++;
                }
            }
        }
        return activeVoices;
    };
private:
};


