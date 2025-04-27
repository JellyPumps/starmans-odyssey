/*
 * Created by Sarthak Rai on 27 Apr 2025.
*/

#pragma once

namespace STARBORN {
  class Scene {
  public:
    virtual ~Scene() = default;
    virtual void init() = 0;
    virtual void update(float delta_time) = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0;

    // ---- Lifecycle ----
    virtual void on_enter() = 0;
    virtual void on_exit() = 0;
  };
}