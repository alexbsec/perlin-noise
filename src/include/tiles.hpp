#ifndef _PERLIN_NOISE_TILES_HPP
#define _PERLIN_NOISE_TILES_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include <string>

const std::string WALL_PATH = "Wall.png";
const std::string FLOOR_PATH = "Floor.png";

namespace textures {
enum ID {
  Wall_TopLeftCorner,
  Wall_TopRightCorner,
  Wall_BottomLeftCorner,
  Wall_BottomRightCorner,
  Wall_Left,
  Wall_Right,
  Floor,
  None
};

enum Type { Wall, Ground, Null };

struct TextureBlock {
  Type type;
  int x;
  int y;
  int index;
  sf::Texture texture;
  sf::Sprite sprite;
  std::vector<TextureBlock*> adjacentBlocks;
 
  TextureBlock(int x, int y)
    : type(Type::Null), x(x), y(y), index(-1), sprite() {}

  TextureBlock(Type type, int x, int y)
    : type(type), x(x), y(y), index(-1), sprite() {}

  void LoadTexture(std::string path) {
    texture.loadFromFile(path);
    sprite.setTexture(texture);
  }

  void SetSpritePosition(float x, float y) {
    sprite.setPosition(x, y);
  }
};
}

#endif
