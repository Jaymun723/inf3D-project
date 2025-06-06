#pragma once

#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "voxel/chunk.hpp"
#include "voxel/chunk_manager.hpp"
#include "car.hpp"

// This definitions allow to use the structures: mesh, mesh_drawable, etc.
// without mentionning explicitly cgp::
using cgp::mesh;
using cgp::mesh_drawable;
using cgp::numarray;
using cgp::timer_basic;
using cgp::vec3;

struct gui_parameters
{
  bool display_frame = false;
  bool display_wireframe = false;
  int render_distance = 3;
};

// The structure of the custom scene
struct scene_structure : cgp::scene_inputs_generic
{

  // ****************************** //
  // Elements and shapes of the scene
  // ****************************** //
  // camera_controller_orbit_euler camera_control;
  camera_controller_orbit_euler camera_control;
  camera_projection_perspective camera_projection;
  window_structure window;

  mesh_drawable global_frame;        // The standard global frame
  environment_structure environment; // Standard environment controler
  input_devices
      inputs;         // Storage for inputs status (mouse, keyboard, window dimension)
  gui_parameters gui; // Standard GUI element storage

  // ****************************** //
  // Elements and shapes of the scene
  // ****************************** //
  ChunkManager manager;
  skybox_drawable skybox;
  Car car;

  // Timer used for the animation
  timer_basic timer;

  int frame_count = 0; // Counter for the number of frames displayed
  float dt;

  // Step counters used for Markov rules
  // int build_tree_step = 0;
  // int build_snake_step = 0;
  // int build_river_step = 0;
  // int build_house_step = 0;

  // ****************************** //
  // Functions
  // ****************************** //

  void initialize(); // Standard initialization to be called before the
                     // animation loop
  void
  display_frame();    // The frame display to be called within the animation loop
  void display_gui(); // The display of the GUI, also called within the
                      // animation loop

  void mouse_move_event();
  void mouse_click_event();
  void keyboard_event(int key, int action);
  void idle_frame();

  void display_info();
};
