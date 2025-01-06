#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cmath>
#include <cstdlib>
#include "include/perlin.hpp"

int main() {
  const int WINDOW_WIDTH = 1920;
  const int WINDOW_HEIGHT = 1080;

  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32),
                          "Perlin Noise");

  const int PIXEL_SIZE = 16;

  const int GRID_WIDTH = WINDOW_WIDTH / PIXEL_SIZE;
  const int GRID_HEIGHT = WINDOW_HEIGHT / PIXEL_SIZE;

  sf::Uint8 *pixels = new sf::Uint8[GRID_WIDTH * GRID_HEIGHT * 4];

  const int GRID_SIZE = 1000;

  for (int x = 0; x < GRID_WIDTH; x++) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
      int index = (y * GRID_WIDTH + x) * 4;

      float val = 0;
      float freq = 1;
      float amp = 1;

      for (int i = 0; i < 12; i++) {
        val += SamplePerlin(x * freq / GRID_SIZE, y * freq / GRID_SIZE);
        freq *= 2;
        amp /= 2;
      }

      // Contrast
      val *= 1.2f;

      // Clipping
      if (val >= 1.0f) {
        val = 1.0f;
      } else if (val < -1.0f) {
        val = -1.0f;
      }

      int color = (int)(((val + 1.0f) * 0.5f) * 255);
      pixels[index] = color;
      pixels[index + 1] = color;
      pixels[index + 2] = color;
      pixels[index + 3] = 255;
    }
  }

  int iterations = 5;
  ApplyCellularAutomata(pixels, GRID_WIDTH, GRID_HEIGHT, iterations);

  sf::Texture texture;
  sf::Sprite sprite;
  texture.create(GRID_WIDTH, GRID_HEIGHT);

  texture.update(pixels);

  sprite.setTexture(texture);
  sprite.setScale(PIXEL_SIZE, PIXEL_SIZE);

  while (window.isOpen()) {
    sf::Event event;
  
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }
    
    window.clear();
    window.draw(sprite);
    window.display();

  }

  delete[] pixels;
  return EXIT_SUCCESS;
}
