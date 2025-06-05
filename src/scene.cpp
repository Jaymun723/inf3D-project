#include "scene.hpp"
#include "markov/rules_examples.hpp"
#include "markov/structures/tree.hpp"
#include "markov/structures/snake.hpp"
#include "markov/structures/river.hpp"
#include "markov/structures/house.hpp"
#include "markov/structures/road.hpp"
#include "markov/structures/crossroads.hpp"
#include "markov/structures/maze_growth.hpp"
#include "voxel/block.hpp"
using namespace cgp;

void scene_structure::initialize() {
    srand((unsigned int)time(NULL));
    camera_control.initialize(inputs,
                            window); // Give access to the inputs and window
                                     // global state to the camera controler
  // camera_control.set_rotation_axis_z();
  camera_control.look_at({3.0f, 2.0f, 2.0f}, {0, 0, 0}, {0, 0, 1});

  display_info();
  global_frame.initialize_data_on_gpu(mesh_primitive_frame());

  // int first_id = manager.AddChunk(vec3(0, 0, 0));
  // int second_id = manager.AddChunk(vec3(2, 0, 0));

  // std::cout << "First id: " << first_id << ", Second id: " << second_id
  //           << std::endl;

    chunk.Load();
}

void scene_structure::display_frame() {
  // Set the light to the current position of the camera
  environment.light = camera_control.camera_model.position();
  ++frame_count;

  if (frame_count % 10 == 0) {
      // build_snake_step = build_snake(chunk, build_snake_step, 1);
      // build_tree_step = build_tree(chunk, build_tree_step, 1);
      // build_river_step = build_river(chunk, build_river_step, 1);
      // build_house_step = build_house(chunk, build_house_step, 5);
	  // build_Xroad_step = build_Xroad(chunk, build_Xroad_step, 1);

	  build_maze_step = build_maze(chunk, build_maze_step, 1);
  }
  

  chunk.Render(environment);
  // manager.Render(environment);

  if (gui.display_frame)
    draw(global_frame, environment);

  if (gui.display_wireframe) {
    if (gui.display_frame) {
      draw_wireframe(global_frame, environment);
    }
    // manager.WireRender(environment);
    chunk.WireRender(environment);
  }

  // Update the current time
  timer.update();
}

void scene_structure::display_gui() {
  ImGui::Checkbox("Frame", &gui.display_frame);
  ImGui::Checkbox("Wireframe", &gui.display_wireframe);
}

void scene_structure::mouse_move_event() {
  if (!inputs.keyboard.shift)
    camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event() {
  camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event() {
  camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame() {
  camera_control.idle_frame(environment.camera_view);
}

void scene_structure::display_info() {
  std::cout << "\nCAMERA CONTROL:" << std::endl;
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << camera_control.doc_usage() << std::endl;
  std::cout << "-----------------------------------------------\n" << std::endl;
}