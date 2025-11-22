/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiToDmxAudioProcessor::MidiToDmxAudioProcessor()
    : AudioProcessor(BusesProperties()
                     .withOutput("Output", juce::AudioChannelSet::stereo(), true))
{}
MidiToDmxAudioProcessor::~MidiToDmxAudioProcessor() = default;

//==============================================================================
const juce::String MidiToDmxAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiToDmxAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MidiToDmxAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MidiToDmxAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MidiToDmxAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MidiToDmxAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MidiToDmxAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MidiToDmxAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MidiToDmxAudioProcessor::getProgramName (int index)
{
    return {};
}

void MidiToDmxAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MidiToDmxAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void MidiToDmxAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiToDmxAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void MidiToDmxAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{ juce::ScopedNoDenormals noDenormals;
//    const int numSamples = buffer.getNumSamples();
//    const double sampleRate = getSampleRate();
    
    // we don't generate audio; clear buffer
    buffer.clear();
    
    // Comment this to only read midi notes
//    sampleCounter += numSamples;
//    if (sampleCounter >= sampleRate)
//    {
//        sampleCounter = 0;
//        int randomNote = 40 + (std::rand() % 40);
//        auto noteOn = juce::MidiMessage::noteOn(1, randomNote, (juce::uint8)100);
//        auto noteOff = juce::MidiMessage::noteOff(1, randomNote);
//        midiMessages.addEvent(noteOn, 0);
//        midiMessages.addEvent(noteOff, (int)(0.4 * sampleRate));
//    }
    
    // process all midi events this block
    for (const auto metadata : midiMessages)
    {
        const auto msg = metadata.getMessage();
        
        if (msg.isNoteOn())
        {
            currentNote = msg.getNoteNumber();
            
            // get readable note name, e.g., "C#4"
            currentNoteName = juce::MidiMessage::getMidiNoteName(currentNote, true, true, 4);
            
            // lookup colour by note name
            currentColour = noteColourMap.getColourForNote(currentNoteName);
            
            
            DBG("Note On: " << currentNoteName << " (" << currentNote << ") -> RGB("
                << (int)currentColour.getRed() << ", "
                << (int)currentColour.getGreen() << ", "
                << (int)currentColour.getBlue() << ")");
        }
        else if (msg.isNoteOff())
        {
            // optional handling
            // isNoteOn = false; // not used here
        }
    }
}

//==============================================================================
bool MidiToDmxAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MidiToDmxAudioProcessor::createEditor()
{
    return new MidiToDmxAudioProcessorEditor (*this);
}

//==============================================================================
void MidiToDmxAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MidiToDmxAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiToDmxAudioProcessor();
}
