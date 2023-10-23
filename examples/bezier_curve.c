#include <SDL2/SDL.h>
#include <math.h>

// Function to draw a quadratic Bézier curve
void drawQuadraticBezier(SDL_Renderer *renderer, int x0, int y0, int x1, int y1,
                         int x2, int y2) {
  // The step size for approximating the curve
  double t;
  for (t = 0.0; t <= 1.0; t += 0.001) {
    double a = pow(1.0 - t, 2);
    double b = 2 * t * (1.0 - t);
    double c = pow(t, 2);
    int x = a * x0 + b * x1 + c * x2;
    int y = a * y0 + b * y1 + c * y2;
    SDL_RenderDrawPoint(renderer, x, y);
  }
}

int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO);

  int width = 800;
  int height = 600;

  SDL_Window *window = SDL_CreateWindow(
      "Curved Line in SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
      width, height, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  SDL_Event e;
  int quit = 0;

  while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT) {
        quit = 1;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color

    // Control points for the Bézier curve
    int x0 = 100, y0 = 300;
    int x1 = 400, y1 = 50;
    int x2 = 700, y2 = 500;

    // Draw the Bézier curve
    drawQuadraticBezier(renderer, x0, y0, x1, y1, x2, y2);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
