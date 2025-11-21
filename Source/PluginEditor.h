#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class MidiToDmxAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                       public juce::FileDragAndDropTarget,
                                       private juce::Timer
{
public:
    MidiToDmxAudioProcessorEditor (MidiToDmxAudioProcessor&);
    ~MidiToDmxAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    MidiToDmxAudioProcessor& processorRef;

    juce::Label noteLabel;

    void timerCallback() override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiToDmxAudioProcessorEditor)
};
