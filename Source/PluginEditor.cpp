#include "PluginEditor.h"
#include "PluginProcessor.h"

MidiToDmxAudioProcessorEditor::MidiToDmxAudioProcessorEditor(MidiToDmxAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    setSize(400, 300);

    // Add a label to display the current note
    addAndMakeVisible(noteLabel);
    noteLabel.setText("No Note", juce::dontSendNotification);
    noteLabel.setJustificationType(juce::Justification::centred);

    startTimerHz(30); // update 30 times per second
}

MidiToDmxAudioProcessorEditor::~MidiToDmxAudioProcessorEditor() = default;

void MidiToDmxAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(processorRef.getCurrentColour()); // background reflects current note colour
}

void MidiToDmxAudioProcessorEditor::resized()
{
    noteLabel.setBounds(10, 50, getWidth() - 20, 30);
}

// Timer updates the note label with the current note info
void MidiToDmxAudioProcessorEditor::timerCallback()
{
    juce::String noteText = processorRef.getCurrentNoteName() + " ("
                            + juce::String(processorRef.currentNote) + ")";
    noteLabel.setText(noteText, juce::dontSendNotification);

    repaint(); // repaint background color
}

bool MidiToDmxAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    // Accept .json files only
    for (auto& f : files)
        if (f.endsWithIgnoreCase(".json"))
            return true;

    return false;
}

void MidiToDmxAudioProcessorEditor::filesDropped(const juce::StringArray& files, int x, int y)
{
    juce::ignoreUnused(x, y);

    juce::File file(files[0]);

    if (file.existsAsFile())
    {
        processorRef.loadNoteColourFile(file);
        DBG("Dropped JSON file loaded: " << file.getFullPathName());

        // Optional: update UI label
        noteLabel.setText("Loaded: " + file.getFileName(), juce::dontSendNotification);
    }
}
