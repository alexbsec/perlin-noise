#include "include/perlin.hpp"
#include "include/tiles.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const int PIXEL_SIZE = 16;
const int GRID_WIDTH = WINDOW_WIDTH / PIXEL_SIZE;
const int GRID_HEIGHT = WINDOW_HEIGHT / PIXEL_SIZE;

void DrawMap(sf::RenderWindow &window,
             const std::map<int, textures::TextureBlock *> &map) {
  for (const auto& pair : map) {
    textures::TextureBlock *block = pair.second;
    block->SetSpritePosition(block->x * PIXEL_SIZE, block->y * PIXEL_SIZE);
    window.draw(block->sprite);
  }
}

int main() {

  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32),
                          "Perlin Noise");


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
  std::map<int, textures::TextureBlock *> textureMap =
      ApplyCellularAutomata(pixels, GRID_WIDTH, GRID_HEIGHT, iterations);

  assert(textureMap.size() == GRID_WIDTH * GRID_HEIGHT);
  assert(textureMap[0]->type == textures::Type::Wall || textures::Type::Ground);

  int walls = 0, grounds = 0, nulls = 0;
  for (const auto &pair : textureMap) {
    if (pair.second->type == textures::Type::Null)
      nulls++;
    if (pair.second->type == textures::Type::Ground)
      grounds++;
    if (pair.second->type == textures::Type::Wall)
      walls++;
  }

  std::cout << "Walls: " << walls << std::endl;
  std::cout << "Grounds: " << grounds << std::endl;
  std::cout << "Nulls: " << nulls << std::endl;

  //sf::Texture texture;
  //sf::Sprite sprite;
  //texture.create(GRID_WIDTH, GRID_HEIGHT);

  //texture.update(pixels);

  //sprite.setTexture(texture);
  //sprite.setScale(PIXEL_SIZE, PIXEL_SIZE);

  while (window.isOpen()) {
    sf::Event event;

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    window.clear();
    DrawMap(window, textureMap);
    window.display();
  }

  delete[] pixels;
  return EXIT_SUCCESS;
}
