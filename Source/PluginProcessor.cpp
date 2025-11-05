/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiToDmxAudioProcessor::MidiToDmxAudioProcessor() : AudioProcessor(BusesProperties()
                                                                    .withOutput("Output", juce::AudioChannelSet::stereo(), true)) {}

MidiToDmxAudioProcessor::~MidiToDmxAudioProcessor()
{
}

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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
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
{
    
    juce::ScopedNoDenormals noDenormals;
    const int numSamples = buffer.getNumSamples();
    const double sampleRate = getSampleRate();
    
    buffer.clear();

    sampleCounter += numSamples;
    if (sampleCounter >= sampleRate){
        sampleCounter = 0;

        int randomNote = 40 + (rand() % 40);
        juce::MidiMessage noteOn = juce::MidiMessage::noteOn(1, randomNote, (juce::uint8)100);
        midiMessages.addEvent(noteOn, 0);

        juce::MidiMessage noteOff = juce::MidiMessage::noteOff(1, randomNote);
        midiMessages.addEvent(noteOff, (int)(0.4 * sampleRate));
    }
    
    
    auto mapNoteToColour = [](int note) -> juce::Colour{
        float t = juce::jlimit(0.0f, 1.0f, note / 127.0f);

        uint red   = (uint)(std::sin(t * juce::MathConstants<float>::pi) * 255);
        uint green = (uint)(std::sin(t * juce::MathConstants<float>::pi * 1.5f) * 255);
        uint blue  = (uint)(std::cos(t * juce::MathConstants<float>::pi) * 255);

        return juce::Colour::fromRGB(red, green, blue);
    };

    for (const auto metadata : midiMessages){
       const auto msg = metadata.getMessage();

       if (msg.isNoteOn())
       {
           currentNote = msg.getNoteNumber();
           currentColour = mapNoteToColour(currentNote);

           DBG("Note On: " << currentNote
               << " -> RGB("
               << (int)currentColour.getRed() << ", "
               << (int)currentColour.getGreen() << ", "
               << (int)currentColour.getBlue() << ")");
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
