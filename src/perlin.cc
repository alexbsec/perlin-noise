#include "include/perlin.hpp"
#include <cmath>
#include <cstdlib>
#include <cstring>

float DotProduct(sf::Vector2f vec1, sf::Vector2f vec2) {
  return vec1.x * vec2.x + vec1.y * vec2.y;
}

float Interpolate(float f1, float f2, float weight) {
  return (f2 - f1) * (3.0f - weight * 2.0f) * weight * weight + f1;
}

sf::Vector2f GetRandomGradient(int ix, int iy) {

  static bool seedInitialized = false;
  if (!seedInitialized) {
    srand(static_cast<unsigned int>(time(0)));
    seedInitialized = true;
  }

  const unsigned w = 8 * sizeof(unsigned);
  const unsigned s = w / 2;
  unsigned a = ix, b = iy;

  // Hashing process
  a *= 3284157443;
  b ^= a << s | a >> (w - s);
  b *= 1911520717;
  a ^= b << s | b >> (w - s);
  a *= 2048419325;

  float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  random *= 2.0f * 3.14159265f;

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

bool IsWithinMapBounds(int x, int y, int width, int height) {
  return x >= 0 && x < width && y >= 0 && y < height;
}

void ApplyCellularAutomata(sf::Uint8 *grid, int width, int height, int count) {
  const float midPoint = -0.7f;
  const int midColor = (int)(((midPoint + 1.0f) * 0.5f) * 255);
  for (int i = 0; i < count; i++) {
    sf::Uint8 *tmpGrid = new sf::Uint8[width * height * 4];
    std::memcpy(tmpGrid, grid, width * height * 4);
    for (int j = 0; j < height; j++) {
      for (int k = 0; k < width; k++) {
        int neighborWallCount = 0;
        for (int y = j - 1; y <= j + 1; y++) {
          for (int x = k - 1; x <= k + 1; x++) {
            if (!IsWithinMapBounds(x, y, width, height)) {
              neighborWallCount++;
              continue;
            }

            if (y == j && x == k)
              continue;

            int index = (y * width + x) * 4;
            if (tmpGrid[index] >= midColor) {
              neighborWallCount++;
            }
          }
        }

        int jkIndex = (j * width + k) * 4;
        if (neighborWallCount > 4) {
          grid[jkIndex] = tmpGrid[jkIndex];
          grid[jkIndex + 1] = tmpGrid[jkIndex + 1];
          grid[jkIndex + 2] = tmpGrid[jkIndex + 2];
          grid[jkIndex + 3] = tmpGrid[jkIndex + 3];
        } else {
          grid[jkIndex] = 0;
          grid[jkIndex + 1] = 0;
          grid[jkIndex + 2] = 0;
          grid[jkIndex + 3] = 255;
        }
      }
    }

    delete[] tmpGrid;
  }
}
