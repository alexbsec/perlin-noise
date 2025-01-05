#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cmath>
#include <cstdlib>

float DotProduct(sf::Vector2f vec1, sf::Vector2f vec2) {
  return vec1.x * vec2.x + vec1.y * vec2.y;
}

float Interpolate(float f1, float f2, float weight) {
  return (f2 - f1) * (3.0f - weight * 2.0f) * weight * weight + f1;
}

sf::Vector2f GetRandomGradient(int ix, int iy) {
  const unsigned w = 8 * sizeof(unsigned);
  const unsigned s = w / 2;
  unsigned a = ix, b = iy;

  // Hashing process
  a *= 3284157443;
  b ^= a << s | a >> (w - s);
  b *= 1911520717;
  a ^= b << s | b >> (w - s);
  a *= 2048419325;

  float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2pi]

  sf::Vector2f vec;
  vec.x = sin(random);
  vec.y = cos(random);

  return vec;
}

float DotGridGradient(int ix, int iy, float x, float y) {
  sf::Vector2f gradient = GetRandomGradient(ix, iy);

  sf::Vector2f distanceVec(x - (float)ix, y - (float)iy);
  return DotProduct(gradient, distanceVec);
}

// Sample Perlin noise at coordinates x and y
float SamplePerlin(float x, float y) {
  // Determine the grid cell corner coordinates
  int x0 = (int)x;
  int y0 = (int)y;
  int x1 = x0 + 1;
  int y1 = y0 + 1;

  // Compute interpolation weights
  float sx = x - (float)x0;
  float sy = y - (float)y0;

  // Compute and interpolate top two corners
  float n0 = DotGridGradient(x0, y0, x, y);
  float n1 = DotGridGradient(x1, y0, x, y);
  float ix0 = Interpolate(n0, n1, sx);

  // Compute and interpolate bottom two corners
  n0 = DotGridGradient(x0, y1, x, y);
  n1 = DotGridGradient(x1, y1, x, y);
  float ix1 = Interpolate(n0, n1, sx);

  // Interpolate between the two previously interpolated values
  float value = Interpolate(ix0, ix1, sy);

  return value;
}

int main() {
  const int WINDOW_WIDTH = 1920;
  const int WINDOW_HEIGHT = 1080;

  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32),
                          "Perlin Noise");

  sf::Uint8 *pixels = new sf::Uint8[WINDOW_WIDTH * WINDOW_HEIGHT * 4];

  const int GRID_SIZE = 400;

  for (int x = 0; x < WINDOW_WIDTH; x++) {
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
      int index = (y * WINDOW_WIDTH + x) * 4;

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
      if (val > 1.0f) {
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

  sf::Texture texture;
  sf::Sprite sprite;
  texture.create(WINDOW_WIDTH, WINDOW_HEIGHT);

  texture.update(pixels);

  sprite.setTexture(texture);

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

  return EXIT_SUCCESS;
}
