/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiToDmxAudioProcessorEditor::MidiToDmxAudioProcessorEditor (MidiToDmxAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    setSize (400, 300);
    startTimerHz(30); // update GUI ~30 FPS
}

MidiToDmxAudioProcessorEditor::~MidiToDmxAudioProcessorEditor() {}

//==============================================================================
void MidiToDmxAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (processorRef.getCurrentColour());

    g.setColour (juce::Colours::white);
    g.setFont (24.0f);
    g.drawFittedText ("Note Colour Visualizer", getLocalBounds(), juce::Justification::centred, 1);
}

void MidiToDmxAudioProcessorEditor::resized() {}

void MidiToDmxAudioProcessorEditor::timerCallback()
{
    repaint();
}
