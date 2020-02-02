#include "GameState.hpp"
#include <onut/Files.h>
#include <nlohmann/json.hpp>

namespace {
  constexpr auto save_file = "savegame.json";
  constexpr auto key_next_level = "next_level";
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
