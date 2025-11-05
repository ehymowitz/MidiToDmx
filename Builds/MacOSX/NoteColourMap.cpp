#include "NoteColourMap.h"

NoteColourMap::NoteColourMap()
{
    // Default colors (in case file isn’t found)
    noteColours.set("C4", juce::Colours::red);
    noteColours.set("D4", juce::Colours::orange);
    noteColours.set("E4", juce::Colours::yellow);
    noteColours.set("F4", juce::Colours::green);
    noteColours.set("G4", juce::Colours::blue);
    noteColours.set("A4", juce::Colours::indigo);
    noteColours.set("B4", juce::Colours::violet);
}

void NoteColourMap::loadFromFile(const juce::File& file)
{
    if (!file.existsAsFile())
    {
        DBG("❌ NoteColourMap: File not found: " << file.getFullPathName());
        return;
    }

    juce::var json = juce::JSON::parse(file);
    if (!json.isObject())
    {
        DBG("❌ NoteColourMap: Invalid JSON structure");
        return;
    }

    noteColours.clear();

    for (auto& key : json.getDynamicObject()->getProperties().getAllKeys())
    {
        auto value = json[key];
        if (value.isArray() && value.getArray()->size() == 3)
        {
            int r = int(value[0]);
            int g = int(value[1]);
            int b = int(value[2]);
            noteColours.set(key, juce::Colour::fromRGB(r, g, b));
        }
    }

    DBG("✅ NoteColourMap loaded " << noteColours.size() << " note colors");
}

juce::Colour NoteColourMap::getColourForNote(const juce::String& noteName) const
{
    if (auto* c = noteColours[keyName(noteName)])
        return *c;

    return juce::Colours::white; // fallback
}
