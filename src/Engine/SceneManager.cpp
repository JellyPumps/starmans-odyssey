/*
 * Created by Sarthak Rai on 27 Apr 2025.
*/

#include "SceneManager.hpp"

namespace STARBORN {
  SceneManager *SceneManager::instance_ = nullptr;

  // ---- Constructor & Destructor ----
  SceneManager::SceneManager()
    : active_scene_(nullptr), next_scene_(nullptr), transitioning_(false) {}

  SceneManager::~SceneManager() { cleanup(); }

  // ---- Scene Management ----
  void SceneManager::add_scene(const std::string &name,
                               std::unique_ptr<Scene> scene) {
    scenes_[name] = std::move(scene);
  }

  bool SceneManager::remove_scene(const std::string &name) {
    if (scenes_.contains(name)) {
      if (active_scene_ == scenes_[name].get()) active_scene_ = nullptr;
      scenes_.erase(name);
      return true;
    }
    return false;
  }

  void SceneManager::set_active_scene(const std::string &name) {
    if (const auto it = scenes_.find(name); it != scenes_.end()) {
      if (active_scene_) {
        active_scene_->on_exit();
        next_scene_ = it->second.get();
        transitioning_ = true;
      } else {
        active_scene_ = it->second.get();
        active_scene_->init();
        active_scene_->on_enter();
      }
    } else throw std::runtime_error("Scene not found: " + name);
  }

  // ---- Scene Methods ----
  void SceneManager::update(float delta_time) {
    if (transitioning_) {
      active_scene_ = next_scene_;
      next_scene_ = nullptr;
      active_scene_->init();
      active_scene_->on_enter();
      transitioning_ = false;
    }

    if (active_scene_) active_scene_->update(delta_time);
  }

  void SceneManager::render() const {
    if (active_scene_) active_scene_->render();
  }

  void SceneManager::cleanup() {
    for (auto &scene_pair : scenes_) {
      scene_pair.second->cleanup();
    }
    scenes_.clear();
    active_scene_ = nullptr;
    next_scene_ = nullptr;
  }
}