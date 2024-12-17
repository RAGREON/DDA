#include <SDL.h>
#include <iostream>
#include <cmath>

class App {
public:
  App(const char* name, int width, int height);
  void get_xy_sign(int* x, int* y, int angle);
  void classic_to_window_x_y(int* x, int* y);
  bool check_boundary(int x, int y);
  void get_angle(int met_x, int met_y);
  void render();
  bool is_running();
  void poll_event();
  void render_slanted_line(int x_offset, int y_offset, int angle, int y_intercept);
  void animate();
  void stop();

private:
  SDL_Window* window;
  SDL_Renderer* renderer;

  bool running;

  int width, height;

  int met_x, met_y, angle;
  bool first_iteration;
};
