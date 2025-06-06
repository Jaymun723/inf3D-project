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

void scene_structure::initialize()
{
  srand((unsigned int)time(NULL));
  camera_control.initialize(inputs,
                            window); // Give access to the inputs and window
  // global state to the camera controler
  camera_control.set_rotation_axis_z();
  camera_control.look_at({0.5f, 0.5f, 0.2f}, {0, 0, 0}, {0, 0, 1});
  // camera_control.camera_model.position_camera = { 3.0f, 2.0f, 20.0f };
  // camera_control.look_at({0, 0.2, 0}, {1, 0.2, 0});
  // camera_control.set_rotation_axis_z();

  display_info();
  global_frame.initialize_data_on_gpu(mesh_primitive_frame());

  // int first_id = manager.AddChunk(vec3(0, 0, 0));

  // std::cout << "First id: " << first_id << std::endl;

  // Chunk &chunk = manager.GetChunk(first_id);
  // faire tes trucs avec chunk

  image_structure image_skybox_template = image_load_file(project::path + "assets/skybox_01.png");

  // Split the image into a grid of 4 x 3 sub-images
  std::vector<image_structure> image_grid = image_split_grid(image_skybox_template, 4, 3);

  skybox.initialize_data_on_gpu();
  // skybox.texture.initialize_cubemap_on_gpu(image_grid[1], image_grid[7], image_grid[5], image_grid[3], image_grid[10], image_grid[4]);
  int i = 0;
  skybox.texture.initialize_cubemap_on_gpu(
      image_grid[4].rotate_90_degrees_clockwise().mirror_vertical(),           // +X
      image_grid[10].rotate_90_degrees_clockwise().mirror_horizontal(),        // -X
      image_grid[1].mirror_horizontal(),                                       // +Y
      image_grid[7].mirror_vertical(),                                         // -Y
      image_grid[5].rotate_90_degrees_counterclockwise().mirror_horizontal(),  // +Z
      image_grid[3].rotate_90_degrees_counterclockwise().mirror_horizontal()); // +Z
}

void scene_structure::display_frame()
{
  // Set the light to the current position of the camera
  environment.light = camera_control.camera_model.position();

  //  Must be called before drawing the other shapes and without writing in the Depth Buffer
  glDepthMask(GL_FALSE); // disable depth-buffer writing
  draw(skybox, environment);
  glDepthMask(GL_TRUE); // re-activate depth-buffer write

  ++frame_count;

  // if (frame_count % 10 == 0)
  // {
  //   // build_snake_step = build_snake(chunk, build_snake_step, 1);
  //   // build_tree_step = build_tree(chunk, build_tree_step, 1);
  //   // build_river_step = build_river(chunk, build_river_step, 1);
  //   build_house_step = build_house(chunk, build_house_step, 5);
  // }

  manager.Update(camera_control.camera_model.position_camera, frame_count);
  manager.Render(environment);

  if (gui.display_frame)
    draw(global_frame, environment);

  if (gui.display_wireframe)
  {
    if (gui.display_frame)
    {
      draw_wireframe(global_frame, environment);
    }
    manager.WireRender(environment);
  }

  // Update the current time
  timer.update();
}

void scene_structure::display_gui()
{
  ImGui::Checkbox("Frame", &gui.display_frame);
  ImGui::Checkbox("Wireframe", &gui.display_wireframe);
  // ImGui::SliderFloat3("Camera position", &gui.camera_position[0], -4, 4);
}

void scene_structure::mouse_move_event()
{
  if (!inputs.keyboard.shift)
    camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event()
{
  camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
  camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame()
{
  camera_control.idle_frame(environment.camera_view);
}

void scene_structure::display_info()
{
  std::cout << "\nCAMERA CONTROL:" << std::endl;
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << camera_control.doc_usage() << std::endl;
  std::cout << "-----------------------------------------------\n"
            << std::endl;
}