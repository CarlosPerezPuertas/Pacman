#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cassert>
#include <vector>
#include <string>



enum class TileType
{
	None,
	Walkable,
	Collidable
};

struct Layer
{
	sf::VertexArray entity_vertex;
	sf::Texture texture;
	int layer_id;
};

struct TMX_Tile
{
	TileType type;
	sf::FloatRect rect;
	sf::FloatRect change_direction_rect;
	sf::Vector2f center;
	int id;
	std::vector<int> adjacent_tiles;
};

struct TMX_Map
{
	sf::Vector2f tile_size;
	sf::Vector2u map_size;
	int num_layers;
	std::string sprite_sheet;
	std::vector<std::vector<int>> maps;
	std::vector<TMX_Tile> collision_map;
};

struct TMX_ObjectGroup
{
	std::vector<sf::Vector2f>  small_dots_positions;
	std::vector<sf::Vector2f>  big_dots_positions;
	std::vector<sf::FloatRect> warp_rects;
	std::vector<sf::Vector2f>  warp_destinies;
};


namespace TMX
{
	class Map : public sf::Drawable, public sf::Transformable
	{
		public:
			Map();

			//Read map from TMX file
			void load(const std::string &filename);

			void read_TMX(const std::string &filename);
			void load_collision_map();
			void load_layers(const std::string spritesheet, std::vector<std::vector<int>> &all_map, unsigned int width, unsigned int height, int layer_id);

			virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
			void draw_layer(sf::RenderTarget &target, sf::RenderStates states, int layer_id) const;

			void scaleMap(sf::Vector2f &factor);
			void setMapPosition(sf::Vector2f pos);
			sf::FloatRect getChangeDirectionRect(int pos);
			inline unsigned int getMapWidth(){ return tmx_info.map_size.x; }
			TMX_ObjectGroup getObjectGroup(){ return object_group; }


		public:
			TMX_Map tmx_info;

		private:
			std::vector<Layer> map_layers;
			TMX_ObjectGroup object_group;
	};
}



