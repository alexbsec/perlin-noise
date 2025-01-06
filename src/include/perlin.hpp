#ifndef _PERLIN_NOISE_PERLIN_HPP
#define _PERLIN_NOISE_PERLIN_HPP

#include "tiles.hpp"
#include <SFML/Graphics.hpp>
#include <map>

float DotProduct(sf::Vector2f vec1, sf::Vector2f vec2);

float Interpolate(float f1, float f2, float weight);

sf::Vector2f GetRandomGradient(int ix, int iy);

float DotGridGradient(int ix, int iy, float x, float y);

float SamplePerlin(float x, float y);

bool IsWithinMapBounds(int x, int y, int width, int height);

std::map<int, textures::TextureBlock *>
ApplyCellularAutomata(sf::Uint8 *grid, int width, int height, int count);

#endif
