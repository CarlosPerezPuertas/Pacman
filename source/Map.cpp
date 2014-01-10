#include "..\header\Map.h"

#include "..\puig_XML\pugiconfig.hpp"
#include "..\puig_XML\pugixml.hpp"

using namespace TMX;

Map::Map() 
{
	tmx_info.num_layers = 0;
	tmx_info.maps.resize(6);
}


Map::~Map()
{

}

//Read map from TMX file
void Map::load(const std::string &filename)
{
	read_TMX(filename);
	
	for (int i = 0; i < 1; i++)
	load_layers("textures/" + tmx_info.sprite_sheet, tmx_info.maps, 30, 33, i);

	scale(sf::Vector2f(1.9f, 1.9f));
	setPosition(sf::Vector2f(55.f, 60.f));
}

void Map::read_TMX(const std::string &filename)
{
	pugi::xml_document doc;
	
	//Load file and check if it's ok, if itsn't then launch assertion
	pugi::xml_parse_result result = doc.load_file(filename.c_str());
	assert(result == true);

	pugi::xml_node root_node;
	assert((root_node = doc.child("map")));

	tmx_info.map_size.x = root_node.attribute("width").as_uint();
	tmx_info.map_size.y = root_node.attribute("height").as_uint();
	tmx_info.tile_size.x = root_node.attribute("tilewidth").as_uint();
	tmx_info.tile_size.y = root_node.attribute("tileheight").as_uint();


	for (pugi::xml_node node = root_node.first_child(); node; node = node.next_sibling())
	{
		std::string node_name = node.name();
		std::cout << node_name << std::endl;

		if (node_name == "tileset")
		{
			pugi::xml_node node_spritesheet = node.child("image");
			std::string node_sprite_name = node_spritesheet.name();

			if (node_sprite_name == "image")
			{
				tmx_info.sprite_sheet = node_spritesheet.attribute("source").value();
			}
		}

		if (node_name == "layer")
		{
			pugi::xml_node node_data = node.child("data");
			std::string node_data_name = node_data.name();
			
			if (node_data_name == "data")
			{
				for (pugi::xml_node node_map = node_data.first_child(); node_map; node_map = node_map.next_sibling())
				{
					tmx_info.maps[tmx_info.num_layers].push_back(node_map.attribute("gid").as_int());
				}
			}
			tmx_info.num_layers++;
		}
	}
}


/*
	We have the tile map read from the file. This is a vector owning the tile number from the spritesheet.
	To construct the map using a vertex array we need to define each tile as a quad(4 vertex)
	
	Each quad is definef by :

	A Position of each vertex
	B Position of the texture vertex in the spritesheet. Info from tile map.

*/
void Map::load_layers(const std::string spritesheet, std::vector<std::vector<int>> &all_map, unsigned int width, unsigned int height, int layer_id)
{
	std::vector<int> &map = all_map[layer_id];

	Layer layer;
	layer.texture.loadFromFile(spritesheet);
	layer.entity_vertex.setPrimitiveType(sf::Quads);
	layer.entity_vertex.resize(width * height * 4); // Each tile have 4 vertex
	layer.layer_id = layer_id;

	sf::Vector2f TS(12.f, 12.f); //TILE_SIZE
	
	for (unsigned int i = 0; i < height; ++i)
	{
		for (unsigned int j = 0; j < width; ++j)
		{
			unsigned int tile_pos = j + i*width;
			unsigned int tile_texture_num = map[tile_pos] - 1;                       //Tiled begins with number 1
			unsigned int TR = tile_texture_num / (layer.texture.getSize().x / static_cast<int>(TS.x)); //TILE_ROW
			unsigned int TC = tile_texture_num % (layer.texture.getSize().x / static_cast<int>(TS.x)); //TILE_COLUMN

			sf::Vertex* quad = &layer.entity_vertex[tile_pos * 4];

			quad[0].position = sf::Vector2f( j * TS.x,        i * TS.y);
			quad[1].position = sf::Vector2f((j + 1) * TS.x,   i * TS.y);
			quad[2].position = sf::Vector2f((j + 1) * TS.x,  (i + 1) * TS.y);
			quad[3].position = sf::Vector2f( j * TS.x,       (i + 1) * TS.y);

			quad[0].texCoords = sf::Vector2f( TC * TS.x,       TR * TS.y);
			quad[1].texCoords = sf::Vector2f((TC + 1) * TS.x,  TR * TS.y);
			quad[2].texCoords = sf::Vector2f((TC + 1) * TS.x, (TR + 1) * TS.y);
			quad[3].texCoords = sf::Vector2f( TC * TS.x,      (TR + 1) * TS.y);
		}
	}

	map_layers.push_back(layer);
}

void Map::draw_layer(sf::RenderTarget &target, sf::RenderStates states, int layer_id) const
{
	states.transform *= getTransform();
	states.texture = &map_layers[layer_id].texture;
	target.draw(map_layers[layer_id].entity_vertex, states);
}



void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	//Check in the parent SceneNode the current layer to draw
	//SceneNode::GetLayer()
	for (int i = 0; i < tmx_info.num_layers; ++i)
	{
		draw_layer(target, states, i);
	}
}





