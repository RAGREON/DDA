#ifndef TRANSFORMATION
#define TRANSFORMATION

#include <SDL.h>
#include <iostream>

class App {
public:
  App(const char* name, int width, int height);
  bool isRunning();
  void poll_event();

  void render();

private:
  SDL_Window* window;
  SDL_Renderer* renderer;

  bool running;

  int width, height;
};

#endif