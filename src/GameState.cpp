#include "GameState.hpp"
#include <onut/Files.h>
#include <nlohmann/json.hpp>

namespace {
    constexpr auto save_file = "savegame.json";
    constexpr auto key_next_level = "next_level";
    const std::string music_choices[]{ "background_music.ogg", "background_music_2.ogg", "ChillDustrial_v0.ogg" };
}

void Progress::load()
{
    *this = {}; // reset to defaults just in case

#if defined(WIN32)
    if (onut::fileExists(save_file))
    {
        try
        {
            auto json = nlohmann::json::parse(onut::getFileData(save_file));
            if (json.contains(key_next_level))
                next_available_level = json.at(key_next_level);
        }
        catch (...)
        {
            // just ignore errors - too bad you save is gone :p
        }
    }
#else
    next_available_level = 99; // congratulations!
#endif
}

void Progress::save()
{
    nlohmann::json out;
    out[key_next_level] = next_available_level;
    onut::createTextFile(save_file, out.dump());
}

MusicPicker::MusicPicker()
    : distribtion(std::size(music_choices))
{
}

std::string const& MusicPicker::getNext_music(std::mt19937& rng)
{
    return music_choices[distribtion(rng)];
}
