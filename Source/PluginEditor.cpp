#include "PluginEditor.h"
#include "PluginProcessor.h"

MidiToDmxAudioProcessorEditor::MidiToDmxAudioProcessorEditor(MidiToDmxAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    setSize(400, 300);

    // Add the load button
    addAndMakeVisible(loadButton);
    loadButton.onClick = [this]
    {
        // Ensure we run on the message thread
        juce::MessageManager::callAsync([this] {
            juce::FileChooser chooser("Select your note_colours.json file",
                                      juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
                                      "*.json");

            chooser.launchAsync(juce::FileBrowserComponent::openMode
                                | juce::FileBrowserComponent::canSelectFiles,
                                [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    processorRef.loadNoteColourFile(file);
                    DBG("✅ Loaded NoteColourMap from: " << file.getFullPathName());
                }
            });
        });
    };

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
    loadButton.setBounds(10, 10, getWidth() - 20, 30);
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

// Drag-and-drop support
void MidiToDmxAudioProcessorEditor::filesDropped(const juce::StringArray& files, int x, int y)
{
    DBG("Files dropped: " << files.joinIntoString(", "));
    for (auto& f : files)
    {
        juce::File file(f);
        if (file.existsAsFile() && file.hasFileExtension("json"))
        {
            processorRef.loadNoteColourFile(file);
            DBG("✅ Loaded NoteColourMap via drag & drop: " << file.getFullPathName());
        }
    }
}
