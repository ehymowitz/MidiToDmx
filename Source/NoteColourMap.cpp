#include "NoteColourMap.h"

NoteColourMap::NoteColourMap()
{
    const std::map<juce::String, juce::Colour> baseColours = {
        // C - Yellow family
        {"C",  juce::Colour::fromRGB(255, 230, 50)},
        {"C#", juce::Colour::fromRGB(255, 210, 80)},
        {"Db", juce::Colour::fromRGB(255, 200, 100)},

        // D - Brown family
        {"D",  juce::Colour::fromRGB(180, 120, 70)},
        {"D#", juce::Colour::fromRGB(160, 100, 60)},
        {"Eb", juce::Colour::fromRGB(140,  90, 50)},

        // E - Green family
        {"E",  juce::Colour::fromRGB( 60, 180,  80)},
        {"E#", juce::Colour::fromRGB( 80, 200, 100)}, // rare but allowed for completeness
        {"Fb", juce::Colour::fromRGB( 40, 150,  70)},

        // F - Grey family
        {"F",  juce::Colour::fromRGB(160, 160, 160)},
        {"F#", juce::Colour::fromRGB(130, 130, 130)},
        {"Gb", juce::Colour::fromRGB(110, 110, 110)},

        // G - Orange family
        {"G",  juce::Colour::fromRGB(255, 150,  50)},
        {"G#", juce::Colour::fromRGB(255, 130,  30)},
        {"Ab", juce::Colour::fromRGB(255, 110,  15)},

        // A - Red family
        {"A",  juce::Colour::fromRGB(255,  80,  80)},
        {"A#", juce::Colour::fromRGB(235,  60,  60)},
        {"Bb", juce::Colour::fromRGB(215,  40,  40)},

        // B - Blue family
        {"B",  juce::Colour::fromRGB( 70, 110, 255)},
        {"B#", juce::Colour::fromRGB(100, 140, 255)},
        {"Cb", juce::Colour::fromRGB( 50,  90, 210)},
    };

    for (int octave = 1; octave <= 6; ++octave)
    {
        for (auto& pair : baseColours)
        {
            auto noteName = pair.first + juce::String(octave);
            noteColours.set(noteName, pair.second);
        }
    }
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
