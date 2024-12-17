#include <iostream>
#include <SDL.h>
#include <vector>
#include <cmath>

std::vector<std::vector<int>> getPlotPoints(int x1, int y1, int x2, int y2) {
  std::vector<std::vector<int>> plotPoints;

  const int delta_x = x2 - x1;
  const int delta_y = y2 - y1;

  const int maxSteps = std::max(std::abs(delta_x), std::abs(delta_y));

  const double inc_x = static_cast<double>(delta_x) / maxSteps;
  const double inc_y = static_cast<double>(delta_y) / maxSteps;

  double x = x1, y = y1;

  for (int i = 0; i <= maxSteps; ++i) {
    int rounded_x = static_cast<int>(std::round(x));
    int rounded_y = static_cast<int>(std::round(y));

    std::vector<int> plotPointPair = { rounded_x, rounded_y };

    plotPoints.push_back(plotPointPair);

    x += inc_x;
    y += inc_y;
  }

  return plotPoints;
}

void render_grid(SDL_Renderer* renderer, int box_size, int x_scale, int y_scale) {
  for (int y = 0; y < y_scale; y++) {
    SDL_Rect box { 0, y * box_size, box_size, box_size };

    for (int x = 0; x < x_scale; x++) {
      box.x = x * box_size;

      SDL_RenderDrawRect(renderer, &box);
    }
  }
}

int get_window_x(int math_x, int start_x, int box_size) {
  return start_x + math_x * box_size;
}

int get_window_y(int math_y, int start_y, int box_size) {
  return start_y - math_y * box_size;
}

void render_dda_point(SDL_Renderer* renderer, int box_size, int x_axis_center, int y_axis_center, std::vector<std::vector<int>> plotPoints) {
  int previous_x = plotPoints[0][0];
  int previous_y = plotPoints[0][1];

  const int start_x = box_size * x_axis_center;
  const int start_y = box_size * y_axis_center;

  int i = 0;

  for (const auto& pair: plotPoints) {
    int previous_window_x = get_window_x(previous_x, start_x, box_size);
    int previous_window_y = get_window_y(previous_y, start_y, box_size);

    int math_x = pair[0];
    int math_y = pair[1];

    int current_window_x = get_window_x(math_x, start_x, box_size);
    int current_window_y = get_window_y(math_y, start_y, box_size);

    // std::cout << i++ << ". " << current_window_x << ", " << current_window_y << std::endl;

    SDL_RenderDrawPoint(renderer, current_window_x, current_window_y);

    SDL_RenderDrawLine(renderer, previous_window_x, previous_window_y, current_window_x, current_window_y);

    previous_x = math_x;
    previous_y = math_y;
  }
}

void input_coordintes(int* x1, int* y1, int* x2, int* y2) {
  std::cout << "enter x1: ";
  std::cin >> *x1;

  std::cout << "enter y1: ";
  std::cin >> *y1;
  
  std::cout << "enter x2: ";
  std::cin >> *x2;
  
  std::cout << "enter y2: ";
  std::cin >> *y2;

  return;
}

int main(int argc, char** argv) {
  int x1, y1;
  int x2, y2;

  input_coordintes(&x1, &y1, &x2, &y2);

  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window* window = SDL_CreateWindow("DDA", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1500, 1000, SDL_WINDOW_SHOWN);

  if (window == nullptr) {
    std::cout << "error";
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, -1);

  if (renderer == nullptr) {
    std::cout << "error";
  } 

  std::vector<std::vector<int>> result = getPlotPoints(x1, y1, x2, y2);

  int i = 1;
  for (const auto& pair: result) {
    std::cout << i++ << ". " << pair[0] << ", " << pair[1] << std::endl;
  }

  bool running = true;  
  int start_x = 3, start_y = 10;
  int box_size = 100;

  while (running) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
      running = false;
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_d:
        start_x--;
        std::cout << "x_offset -> +1\n";
        break;
      case SDLK_a:
        start_x++;
        std::cout << "x_offset -> -1\n";
        break;
      case SDLK_w:
        start_y++;
        std::cout << "y_offset -> +1\n";
        break;
      case SDLK_s:
        start_y--;
        std::cout << "y_offset -> -1\n";
        break;
      case SDLK_e:
        std::cout << "decreased box size: " << box_size << " -> " << box_size - 10 << std::endl;
        box_size -=10;
        break;
      case SDLK_q:
        std::cout << "increased box size: " << box_size << " -> " << box_size + 10 << std::endl;
        box_size +=10;
        break;
      case SDLK_i:
        input_coordintes(&x1, &y1, &x2, &y2);
        result.clear();
        result = getPlotPoints(x1, y1, x2, y2);
        break;
      }
    default:
      break;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer); // Clear the screen

    // Redraw grid and DDA points
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set color for grid and points (black)
    // render_grid(renderer, 100, 15, 10);             // Redraw the grid
    render_dda_point(renderer, box_size, start_x, start_y, result); // Redraw the DDA line

    SDL_RenderPresent(renderer); // Present the new frame
    
  }

  return 0;
}
