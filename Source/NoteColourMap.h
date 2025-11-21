#pragma once
#include <JuceHeader.h>

class NoteColourMap
{
public:
    NoteColourMap();

    // Load from a JSON file
    void loadFromFile(const juce::File& file);

    // Get colour by note name (returns white if not found)
    juce::Colour getColourForNote(const juce::String& noteName) const;

private:
    juce::HashMap<juce::String, juce::Colour> noteColours;
};
