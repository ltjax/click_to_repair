#pragma once
#include <memory>
#include "Screen.hpp"
#include <random>
#include "MultipleOutcomePrng.hpp"

struct Progress
{
    int next_available_level = 0;
};

struct MusicPicker
{
    MusicPicker();
    std::string const& getNext_music(std::mt19937& rng);
private:
    CMultipleOutcomePrng distribtion;

};

struct SharedState
{
    Progress progress;
    std::mt19937 rng;
    MusicPicker music;

    void load();
    void save();
};

struct GameState
{
    std::unique_ptr<Screen> menu_state;
    std::shared_ptr<SharedState> shared_state;
};
