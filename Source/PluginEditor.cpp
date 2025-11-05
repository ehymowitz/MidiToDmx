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
    
    addAndMakeVisible(noteLabel);
    noteLabel.setFont(juce::FontOptions(24.0f, juce::Font::bold));
    noteLabel.setJustificationType(juce::Justification::centred);
}

MidiToDmxAudioProcessorEditor::~MidiToDmxAudioProcessorEditor() {}

//==============================================================================
void MidiToDmxAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (processorRef.getCurrentColour());

    g.setColour (juce::Colours::white);
    g.setFont (24.0f);
    auto text = processorRef.getCurrentNoteName() + " (" +
                    juce::String(processorRef.getCurrentNoteNumber()) + ")";
        g.drawFittedText(text, getLocalBounds(), juce::Justification::centred, 1);}

void MidiToDmxAudioProcessorEditor::resized() {
    noteLabel.setBounds(getLocalBounds());
}

void MidiToDmxAudioProcessorEditor::timerCallback()
{
    repaint();
}

