/*
  ==============================================================================

    SampleSynth.h
    Created: 1 Jul 2024 8:58:34pm
    Author:  Tobias Koelker

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SampleSound.h"
#include "SampleVoice.h"

class SampleSynth : public juce::Synthesiser
{
public:
    SampleSynth()
    {
        this->clearVoices();
        for (int i = 0; i < 4; ++i)
            this->addVoice(new juce::SamplerVoice());

        juce::AudioFormatManager formatManager;
        formatManager.registerBasicFormats();

        // Use BinaryData to get the WAV file
        std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(
                std::make_unique<juce::MemoryInputStream>(BinaryData::cello_wav, BinaryData::cello_wavSize, false)
        ));
        
        if (reader != nullptr)
        {
            auto sampleLength = static_cast<int>(reader->lengthInSamples);
            juce::AudioBuffer<float> tempBuffer(1, sampleLength);
            reader->read(&tempBuffer, 0, sampleLength, 0, true, false);
            
            juce::BigInteger allNotes;
            allNotes.setRange(0, 128, true);

            sampleSound = new juce::SamplerSound("Cello", *reader, allNotes, 60, 0.1, 0.1, 10.0);
            this->addSound(sampleSound);
            //delete reader;
        }
    };
    
    ~SampleSynth() {};
private:
    juce::SamplerSound::Ptr sampleSound;
};
