/*
 * Created by Sarthak Rai on 27 Apr 2025.
*/

#pragma once

#include "Scene.hpp"
#include <unordered_map>
#include <memory>
#include <string>

namespace STARBORN {
  class SceneManager {
  private:
    static SceneManager *instance_;
    std::unordered_map<std::string, std::unique_ptr<Scene>> scenes_;
    Scene *active_scene_;
    Scene *next_scene_;
    bool transitioning_;

    SceneManager();
  public:
    // ---- Singleton Instance ----
    SceneManager(const SceneManager &) = delete;
    SceneManager &operator=(const SceneManager &) = delete;
    ~SceneManager();

    static SceneManager &get_instance() {
      if (instance_ == nullptr) instance_ = new SceneManager();
      return *instance_;
    }

    // ---- Scene Management ----
    void add_scene(const std::string &name, std::unique_ptr<Scene> scene);
    bool remove_scene(const std::string &name);
    void set_active_scene(const std::string &name);

    // ---- Scene Methods ----
    void update(float delta_time);
    void render() const;
    void cleanup();

    // ---- Getters ----
    Scene *get_active_scene() const { return active_scene_; };
  };
}