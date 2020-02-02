#include "GameState.hpp"
#include "Constants.hpp"
#include <onut/Files.h>
#include <nlohmann/json.hpp>

namespace {
    constexpr auto save_file = "savegame.json";
    constexpr auto key_next_level = "next_level";
    const std::string music_choices[]{ "background_music.ogg", "background_music_2.ogg", "ChillDustrial_v0.ogg" };
}

void SharedState::load()
{
    progress = {}; // reset to defaults just in case

#if defined(WIN32)
    if (onut::fileExists(save_file))
    {
        auto json = nlohmann::json::parse(onut::getFileData(save_file));
        if (json.contains(key_next_level))
            progress.next_available_level = json.at(key_next_level);
    }
#else
    progress.next_available_level = Constants::MAX_LEVELS(); // congratulations!
#endif
}

void SharedState::save()
{
    nlohmann::json out;
    out[key_next_level] = progress.next_available_level;
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
