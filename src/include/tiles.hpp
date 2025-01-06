#ifndef _PERLIN_NOISE_TILES_HPP
#define _PERLIN_NOISE_TILES_HPP

#include <SFML/Graphics/Sprite.hpp>
#include <vector>

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
  sf::Sprite sprite;
  std::vector<TextureBlock*> adjacentBlocks;
 
  TextureBlock(int x, int y)
    : type(Type::Null), x(x), y(y), index(-1), sprite() {}

  TextureBlock(Type type, int x, int y)
    : type(type), x(x), y(y), index(-1), sprite() {}
};
}

#endif
