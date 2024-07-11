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
    gameOfLife(20), midiMapper(gameOfLife), kickSynth(SampleType::Kick), hhSynth(SampleType::ClosedHH)
{
    _current16thStep = 0;
    _bpm = 120;
    startTimer(200);
}

GameOfLifeAudioProcessor::~GameOfLifeAudioProcessor()
{
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
    kickSynth.setCurrentPlaybackSampleRate(sampleRate);
    hhSynth.setCurrentPlaybackSampleRate(sampleRate);
    sinewaveSynth.setCurrentPlaybackSampleRate(sampleRate);
    reverb.setCurrentPlaybackSampleRate(sampleRate);
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

//    if (flag)
//    {
//        if (synth.playAt16thNote(current16thNote))
//        {
//            // Generate a MIDI note-on message (middle C, velocity 0.7)
//            juce::MidiMessage noteOn = juce::MidiMessage::noteOn(1, 60, 0.7f);
//
//            // Insert the MIDI messages into the midiMessages buffer
//            midiMessages.addEvent(noteOn, 0);
//        }
//
//        flag = false;
//    }
//
//    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
//    if(flag)
//    {
//        //juce::MidiMessage noteOn = juce::MidiMessage::noteOn(1, 60, 0.7f);
//        //midiMessages.addEvent(noteOn, 0);
//
//        midiMapper.addMidiMessagesToBuffer(midiMessages);
//
//        flag = false;
//    }
//
//    juce::MidiBuffer filteredMidiMessagesChannel1;
//    juce::MidiBuffer filteredMidiMessagesChannel2;
//
//    // Iterate through the MIDI messages
//    for (const auto metadata : midiMessages)
//    {
//        auto message = metadata.getMessage();
//        if (message.getChannel() == 1)
//        {
//            filteredMidiMessagesChannel1.addEvent(message, metadata.samplePosition);
//        }
//        if (message.getChannel() == 2)
//        {
//            filteredMidiMessagesChannel2.addEvent(message, metadata.samplePosition);
//        }
//    }
//
//    kickSynth.renderNextBlock(buffer, filteredMidiMessagesChannel1, 0, buffer.getNumSamples());
    
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
    
    juce::MidiBuffer kickMidiMessages;
    juce::MidiBuffer hhMidiMessages;
    
    if (auto* playHead = getPlayHead())
    {
        auto position = playHead->getPosition();
        if (position)
        {
            if (position->getBpm().hasValue())
            {
                _bpm = static_cast<int>(*position->getBpm());
            }
            if (position->getIsPlaying())
            {
                if (position->getPpqPosition().hasValue())
                {
                    _ppq = static_cast<double>(*position->getPpqPosition());
                    
                    const double quarterNoteDuration = 60.0 / _bpm.get();
                    const double samplesPerQuarterNote = getSampleRate() * quarterNoteDuration;
                    
                    const double samplesPerStep = samplesPerQuarterNote / stepsPerBeat;
                    
                    int last16thStep = _current16thStep.get();
                    _current16thStep = static_cast<int>(_ppq.get() * stepsPerBeat) % numSteps;
                    
                    if (_current16thStep.get() == 0 && _current16thStep.get() != last16thStep)
                    {
                        flag = true;
                    }
                    
                    const int samplePositionInStep = static_cast<int>((_ppq.get() * stepsPerBeat) * samplesPerStep) % static_cast<int>(samplesPerStep);
                    
                    // We process each sample in the block to check if a note should be triggered
                    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
                    {
                        if (sample == samplePositionInStep)
                        {
                            // Send note-on message
                            juce::MidiMessage noteOn = juce::MidiMessage::noteOn(1, 60, (juce::uint8)60);
                            noteOn.setTimeStamp(sample);
                            
                            if (kickSynth.playAt16thNote(_current16thStep.get()))
                            {
                                kickMidiMessages.addEvent(noteOn, sample);
                            }
                            if (hhSynth.playAt16thNote(_current16thStep.get()))
                            {
                                hhMidiMessages.addEvent(noteOn, sample);
                            }
                            // On every first beat in a bar
                            if (_current16thStep.get() == 0 && _current16thStep.get() != last16thStep)
                            {
                                sinewaveSynth.allNotesOff(2, true);
                                midiMapper.addMidiMessagesToBuffer(midiMessages);
                            }
                        }
                    }
                }
            }
            else // if not playing
            {
                sinewaveSynth.allNotesOff(2, true);
            }
        }
    }
    
    // create synth which is using midiMessages to create sound!!!
    sinewaveSynth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    // add effect only to sinewave synth sound
    reverb.process(buffer, buffer.getNumSamples());
    
    kickSynth.renderNextBlock(buffer, kickMidiMessages, 0, buffer.getNumSamples());
    hhSynth.renderNextBlock(buffer, hhMidiMessages, 0, buffer.getNumSamples());
    
    // clear midi messages just in case there is something in there
    midiMessages.clear();
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
    if (flag)
    {
        gameOfLife.doNextStep();
        flag = false;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GameOfLifeAudioProcessor();
}
