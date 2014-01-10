#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cassert>
#include <vector>
#include <string>


struct Layer
{
	sf::VertexArray entity_vertex;
	sf::Texture texture;
	int layer_id;
};

struct TMX_Info
{
	sf::Vector2u tile_size;
	sf::Vector2u map_size;
	int num_layers;
	std::string sprite_sheet;
	std::vector<std::vector<int>> maps;
};


namespace TMX
{
	class Map : public sf::Drawable, public sf::Transformable
	{
		public:

			Map();
			~Map();

			//Read map from TMX file
			void load(const std::string &filename);

			void read_TMX(const std::string &filename);
			void load_layers(const std::string spritesheet, std::vector<std::vector<int>> &all_map, unsigned int width, unsigned int height, int layer_id);
		
			virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
			void draw_layer(sf::RenderTarget &target, sf::RenderStates states, int layer_id) const;


		private:

			std::vector<Layer> map_layers;
			TMX_Info tmx_info;
	};
}



