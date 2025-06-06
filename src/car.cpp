#include "car.hpp"
#include <iostream>

const vec3 Car::camera_offset = {-1, 0, 1};

void Car::Initialize(camera_controller_orbit_euler &camera)
{
  m_position.z = 0.2f;
  mesh car = mesh_primitive_cube({0, 0, 0}, 0.2);
  car.push_back(
      mesh_primitive_cube({0.1, -0.05, -0.05}, 0.1));
  car.push_back(
      mesh_primitive_cube({0.1, 0.05, -0.05}, 0.1));

  m_car_mesh.initialize_data_on_gpu(car);

  camera.set_rotation_axis_z();
  camera.look_at(camera_offset, {0, 0, 0}, {0, 0, 1});
}

const float MAX_SPEED = 2.0f;

void Car::Update(camera_controller_orbit_euler &camera, float dt)
{

  m_speed += m_acceleration * 0.05;
  if (m_speed > MAX_SPEED)
  {
    m_speed = MAX_SPEED;
  }
  else if (m_speed < -MAX_SPEED)
  {
    m_speed = -MAX_SPEED;
  }

  float speed = m_speed;
  if (std::abs(speed) < 0.1)
  {
    speed = 0;
  }

  m_angle += m_orientation * 1.2 * dt;

  m_position.x += speed * cos(m_angle) * dt;
  m_position.y += speed * sin(m_angle) * dt;
  // if (norm(m_direction) < 0.1)
  // {
  //   return;
  // }
  // vec2 dir = normalize(m_direction);
  // m_position.x += dir.x * dt;
  // m_position.y += dir.y * dt;

  // camera.look_at(m_position - camera_offset, m_position, {0, 0, 1});
  camera.camera_model.center_of_rotation = m_position;
}

void Car::Render(const environment_structure &environment)
{
  m_car_mesh.model.translation = m_position;
  m_car_mesh.model.set_rotation(
      m_car_mesh.model.rotation.from_axis_angle({0, 0, 1}, m_angle));
  draw(m_car_mesh, environment);
}

void Car::WireRender(const environment_structure &environment)
{
  m_car_mesh.model.translation = m_position;
  draw_wireframe(m_car_mesh, environment);
}

void Car::HandleKeyboard(const environment_structure &environment, int key, int action)
{
  // std::cout << key << std::endl;
  switch (key)
  {
  case GLFW_KEY_W:
    if (action != 0)
    {
      m_acceleration = 1;
    }
    else
    {
      m_acceleration = 0;
    }
    break;
  case GLFW_KEY_S:
    if (action != 0)
    {
      m_acceleration = -1;
    }
    else
    {
      m_acceleration = 0;
    }
    break;
  case GLFW_KEY_A:
    if (action != 0)
    {
      m_orientation = 1;
    }
    else
    {
      m_orientation = 0;
    }
    break;

  case GLFW_KEY_D:
    if (action != 0)
    {
      m_orientation = -1;
    }
    else
    {
      m_orientation = 0;
    }
    break;

  default:
    break;
  }
}

vec3 Car::GetLightPosition()
{
  vec3 front = {
      cos(m_angle), sin(m_angle), -0.05};

  return m_position + front * 0.23;
}
