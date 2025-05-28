#include "scene.hpp"
#include "markov/rules_examples.hpp"
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

  // int first_id = manager.AddChunk(vec3(0, 0, 0));
  // int second_id = manager.AddChunk(vec3(2, 0, 0));

  // std::cout << "First id: " << first_id << ", Second id: " << second_id
  //           << std::endl;

  
  //appear.applyRule(chunk, 1);
  //build_base.applyRule(chunk, -1);
  //elevate.applyRule(chunk, -1);
  //chunk.RandomChunk(0.3f);
  chunk.m_pBlocks[10][10][10].block_type = BlockType_Head;
  chunk.Load();
}

void scene_structure::display_frame() {
  // Set the light to the current position of the camera
  environment.light = camera_control.camera_model.position();

  // keep_only_full.applyRule(chunk, 1);
  // testLeftDisappear.applyRule(chunk, 1);

  if (!AppearOnHead.applyRule(chunk, 3)) {
      int random_x = rand() % chunk.CHUNK_SIZE;
      int random_y = rand() % chunk.CHUNK_SIZE;
      int random_z = rand() % chunk.CHUNK_SIZE;
      chunk.m_pBlocks[random_x][random_y][random_z].block_type = BlockType_Head;
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