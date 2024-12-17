#include "App.hpp"

App::App(const char* name, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialized!\nError: " << SDL_GetError() << std::endl;
        exit(1);
    }

    running = true;
    first_iteration = true;

    this->width = width;
    this->height = height;

    this->angle = 60;

    this->met_x = width / 2;
    this->met_y = height / 2;

    window = SDL_CreateWindow(
        name,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "Error initializing renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
}

void App::get_xy_sign(int* x, int* y, int angle) {
    int x_sign = -1, y_sign = -1;

    if (angle >= 0 && angle < 90) {
        x_sign = 1;
        y_sign = 1;
    } 
    else if (angle >= 90 && angle < 180) {
        x_sign = -1;
        y_sign = 1;
    } 
    else if (angle >= 180 && angle < 270) {
        x_sign = -1;
        y_sign = -1;
    }
    else if (angle >= 270 && angle < 360) {
        x_sign = 1;
        y_sign = -1;
    }

    *x = x_sign;
    *y = y_sign;
}

void App::classic_to_window_x_y(int* x, int* y) {
    *y = this->height - *y;  // Corrected to adjust 'y', not 'x'
}

bool App::check_boundary(int x, int y) {
    if (x < 0 || x > width || y < 0 || y > height) {
        if (x <= 0) {
            met_x = 0;
            met_y = y;
        }
        else if (x >= this->width) {
            met_x = this->width;
            met_y = y;
        }

        if (y <= 0) {
            met_y = 0;
            met_x = x;
        }
        else if (y >= this->height) {
            met_y = this->height;
            met_x = x;
        }
        
        return true;
    }

    return false;
}

void App::get_angle(int met_x, int met_y) {
    if (first_iteration) {
        first_iteration = false;
        return;
    };

    std::cout << met_x << ", " << met_y << std::endl;

    if (met_x >= this->width) {
        angle = 135;  // Reflect off the right boundary
        return;
    }
    
    if (met_x < 0) {
        angle = 315;  // Reflect off the left boundary
        return;
    }

    if (met_y < 0) {
        angle = 200;  // Reflect off the top boundary
        return;
    }

    if (met_y >= this->height) {
        angle = 75;  // Reflect off the bottom boundary
    }
}

void App::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);  // Clear previous frame

    animate();

    SDL_RenderPresent(renderer);  // Present the updated frame
}

bool App::is_running() {
    return running;
}

void App::poll_event() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            this->running = false;
            break;
        
        default:
            break;
        }
    }
}

void App::render_slanted_line(int x_offset, int y_offset, int angle, int y_intercept) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    double x = 0.00, y = 0.00, c = y_intercept, slope = static_cast<double>(tan(angle * M_PI / 180));

    int window_x, window_y;
    int sign_x, sign_y;

    do {
        y = slope * x + c;

        get_xy_sign(&sign_x, &sign_y, angle);

        window_x = static_cast<int>(sign_x * x + x_offset);
        window_y = static_cast<int>(sign_y * y + y_offset);

        classic_to_window_x_y(&window_x, &window_y);
        
        SDL_RenderDrawPoint(renderer, window_x, window_y);

        x++;
    } while (!check_boundary(window_x, window_y));
}

void App::animate() {
    get_angle(met_x, met_y);

    render_slanted_line(met_x, met_y, angle, 2);  // Animating a line
}

void App::stop() {
    running = false;
}
