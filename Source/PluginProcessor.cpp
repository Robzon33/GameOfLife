/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"

//==============================================================================
GameOfLifeAudioProcessor::GameOfLifeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    gameOfLife(20)
{
    current16thNote = 0;
}

GameOfLifeAudioProcessor::~GameOfLifeAudioProcessor()
{
    this->bpm = 50;
}

//==============================================================================
const juce::String GameOfLifeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GameOfLifeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GameOfLifeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GameOfLifeAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GameOfLifeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GameOfLifeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GameOfLifeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GameOfLifeAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GameOfLifeAudioProcessor::getProgramName (int index)
{
    return {};
}

void GameOfLifeAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GameOfLifeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    
    // set up bpm
    bpm = 50;
    
    // If there's a playhead and it has a tempo, use it
    if (auto* playHead = getPlayHead())
    {
        auto position = playHead->getPosition();
        if (position)
        {
            bpm = static_cast<int>(*position->getBpm());
        }
    }

    // Log or print the BPM to ensure it's being set correctly
    DBG("BPM set to: " << bpm);
    
    // Calculate timer interval based on BPM
    double secondsPerBeat = 60.0 / bpm;
    double secondsPer16thNote = secondsPerBeat / 4.0;
            
    // Convert seconds to milliseconds for Timer
    int timerInterval = static_cast<int>(secondsPer16thNote * 1000.0);

    startTimer(timerInterval);  // Start the timer with calculated interval
    
    //    // Playhead stuff
    //    auto playhead = getPlayHead();
    //    if (playhead != nullptr)
    //    {
    //        auto position = playhead->getPosition();
    //        if(position)
    //        {
    //            auto bpm = position->getBpm();
    //            auto barCount = position->getBarCount();
    //            auto isPlaying = position->getIsPlaying();
    //            auto isLoop = position->getIsLooping();
    //            auto ppq = position->getPpqPosition();
    //            if (ppq)
    //            {
    //                // Dont do this on audio thread!!!
    //                DBG ("PPQ: " << *ppq);
    //            }
    //        }
    //    }
}

void GameOfLifeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GameOfLifeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GameOfLifeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (flag)
    {
        if (synth.playAt16thNote(current16thNote))
        {
            // Generate a MIDI note-on message (middle C, velocity 0.7)
            juce::MidiMessage noteOn = juce::MidiMessage::noteOn(1, 60, 0.7f);

            // Insert the MIDI messages into the midiMessages buffer
            midiMessages.addEvent(noteOn, 0);
        }
        
        flag = false;
    }
    
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
      //  auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    //}
    

}

//==============================================================================
bool GameOfLifeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GameOfLifeAudioProcessor::createEditor()
{
    return new GameOfLifeAudioProcessorEditor (*this, gameOfLife);
}

//==============================================================================
void GameOfLifeAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GameOfLifeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void GameOfLifeAudioProcessor::timerCallback()
{
    current16thNote = current16thNote + 1;
    
    if (current16thNote == 16)
        current16thNote = 0;
    
    flag = true;
    
    if (current16thNote == 0)
    {
        gameOfLife.doNextStep();
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GameOfLifeAudioProcessor();
}
