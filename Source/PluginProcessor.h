#pragma once

#include <JuceHeader.h>
#include "NoteColourMap.h"

//==============================================================================
class MidiToDmxAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MidiToDmxAudioProcessor();
    ~MidiToDmxAudioProcessor() override;

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

    // Accessors for the editor
    juce::Colour getCurrentColour() const noexcept { return currentColour; }
    juce::String getCurrentNoteName() const noexcept { return currentNoteName; }
    int getCurrentNoteNumber() const noexcept { return currentNote; }
    
    void loadNoteColourFile(const juce::File& file) { noteColourMap.loadFromFile(file); }
    int currentNote = -1;

private:
    NoteColourMap noteColourMap;

    juce::String currentNoteName = "None";

    double currentFrequency = 0.0;
    double phase = 0.0;
    bool isNoteOn = false;
    double sampleCounter = 0.0;

    juce::Colour currentColour = juce::Colours::black;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiToDmxAudioProcessor)
};
