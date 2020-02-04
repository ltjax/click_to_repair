#include "GameState.hpp"
#include "Constants.hpp"
#include <onut/Files.h>
#include <nlohmann/json.hpp>

namespace {
    constexpr auto save_file = "savegame.json";
    constexpr auto key_next_level = "next_level";
    const std::string music_choices[]{ "background_music.ogg", "background_music_2.ogg", "ChillDustrial_v0.ogg" };
    constexpr auto key_is_fullscreen = "is_fullscreen";
}

void SharedState::load()
{
    progress = {}; // reset to defaults just in case

    if (onut::fileExists(save_file))
    {
        auto json = nlohmann::json::parse(onut::getFileData(save_file));
        auto get = [&json](auto& var, auto name) {
            if (json.contains(name))
                var = json.at(name);
        };
        get(progress.next_available_level, key_next_level);
        get(is_fullscreen, key_is_fullscreen);
    }
}

void SharedState::save()
{
    nlohmann::json out;
    out[key_next_level] = progress.next_available_level;
    out[key_is_fullscreen] = is_fullscreen;
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
