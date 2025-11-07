#include "NoteColourMap.h"

NoteColourMap::NoteColourMap()
{
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
        DBG("❌ NoteColourMap::loadFromFile - file not found: " << file.getFullPathName());
        return;
    }

    juce::String jsonText = file.loadFileAsString();
    juce::var parsed = juce::JSON::parse(jsonText);

    if (!parsed.isObject())
    {
        DBG("❌ NoteColourMap::loadFromFile - not a valid JSON object");
        return;
    }

    noteColours.clear();

    auto* dynObj = parsed.getDynamicObject();
    if (dynObj == nullptr)
    {
        DBG("❌ NoteColourMap::loadFromFile - dynamic object is null");
        return;
    }

    for (const auto& prop : dynObj->getProperties())
    {
        juce::String noteName = prop.name.toString();
        juce::var value = prop.value;

        if (value.isArray())
        {
            auto* arr = value.getArray();

            if (arr->size() >= 3)
            {
                int r = (int)arr->getUnchecked(0);
                int g = (int)arr->getUnchecked(1);
                int b = (int)arr->getUnchecked(2);

                noteColours.set(noteName, juce::Colour::fromRGB((uint)r, (uint)g, (uint)b));
            }
        }
    }

    DBG("NoteColourMap loaded " << noteColours.size() << " entries from " << file.getFileName());
}

juce::Colour NoteColourMap::getColourForNote(const juce::String& noteName) const
{
    if (noteColours.contains(noteName))
        return noteColours[noteName];

    return juce::Colours::white;
}
