#pragma once

#include "core/include/core/entities/Song.hpp"
#include "core/include/core/interfaces/IPlayable.hpp"
#include "core/include/core/interfaces/IPlayableObject.hpp"
#include <memory>
#include <vector>

class MockPlayable : public core::IPlayable {
private:
  std::vector<std::shared_ptr<core::Song>> _songs;
  int _id;

public:
  MockPlayable(const std::vector<std::shared_ptr<core::Song>> &songs,
               int id = 1)
      : _songs(songs), _id(id) {}

  std::vector<std::shared_ptr<core::IPlayableObject>>
  getPlayableObjects() const override {
    std::vector<std::shared_ptr<core::IPlayableObject>> playableObjects;
    for (const auto &song : _songs) {
      playableObjects.push_back(
          std::static_pointer_cast<core::IPlayableObject>(song));
    }
    return playableObjects;
  }
};
