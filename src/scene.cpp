#include "scene.hpp"
#include "voxel/block.hpp"

using namespace cgp;

void scene_structure::initialize() {
  camera_control.initialize(inputs,
                            window); // Give access to the inputs and window
                                     // global state to the camera controler
  camera_control.set_rotation_axis_z();
  camera_control.look_at({3.0f, 2.0f, 2.0f}, {0, 0, 0}, {0, 0, 1});

  display_info();
  global_frame.initialize_data_on_gpu(mesh_primitive_frame());
}

void scene_structure::display_frame() {
  // Set the light to the current position of the camera
  environment.light = camera_control.camera_model.position();

  if (gui.display_frame)
    draw(global_frame, environment);

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