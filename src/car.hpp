#pragma once
#include "cgp/cgp.hpp"
#include "environment.hpp"

using namespace cgp;

class Car
{
public:
  void Initialize(camera_controller_orbit_euler &camera);

  vec3 m_position;

  static const vec3 camera_offset;
  void Update(camera_controller_orbit_euler &camera, float dt);

  void Render(const environment_structure &environment);
  void WireRender(const environment_structure &environment);

  void HandleKeyboard(const environment_structure &environment, int key, int action);

  float m_speed;
  float m_angle;

  vec3 GetLightPosition();

private:
  mesh_drawable m_car_mesh;

  int m_acceleration;
  int m_orientation;
};