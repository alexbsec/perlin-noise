#ifndef _PERLIN_NOISE_PERLIN_HPP
#define _PERLIN_NOISE_PERLIN_HPP

#include <SFML/Graphics.hpp>

float DotProduct(sf::Vector2f vec1, sf::Vector2f vec2);

float Interpolate(float f1, float f2, float weight);

sf::Vector2f GetRandomGradient(int ix, int iy);

float DotGridGradient(int ix, int iy, float x, float y);

float SamplePerlin(float x, float y);

bool IsWithinMapBounds(int x, int y, int width, int height);

void ApplyCellularAutomata(sf::Uint8 *grid, int width, int height, int count);

#endif
