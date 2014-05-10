#include "..\header\Map.h"

#include "..\puig_XML\pugiconfig.hpp"
#include "..\puig_XML\pugixml.hpp"

using namespace TMX;

Map::Map() 
: active(true)
, time_disabled(sf::Time::Zero)
, isTemporalDisabled(false)
{
	tmx_info.num_layers = 0;
	tmx_info.maps.resize(6);
}


//Read map from TMX file
void Map::load(const std::string &filename)
{
	read_TMX(filename);
	
	for (int i = 0; i < tmx_info.num_layers; i++)
	load_layers("textures/" + tmx_info.sprite_sheet, tmx_info.maps, 30, 33, 0);

	load_collision_map();
	scaleMap(sf::Vector2f(2.f, 2.f));
	//setMapPosition(sf::Vector2f(40.f, 60.f));
	setMapPosition(sf::Vector2f(0.f, 0.f));
	
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
	tmx_info.tile_size.x = root_node.attribute("tilewidth").as_float();
	tmx_info.tile_size.y = root_node.attribute("tileheight").as_float();

	for (pugi::xml_node node = root_node.first_child(); node; node = node.next_sibling())
	{
		std::string node_name = node.name();
		std::string object_name = node.attribute("name").as_string();

		//Read the tileset name and store it as string
		//The colision layer is not read
		if (node_name == "tileset" && object_name != "Collision")
		{
			if (object_name == "pacman sprite-sheet definitivo")
			{
				pugi::xml_node node_spritesheet = node.child("image");
				std::string node_sprite_name = node_spritesheet.name();

				if (node_sprite_name == "image")
				{
					tmx_info.sprite_sheet = node_spritesheet.attribute("source").value();
				}
			}	
		}

		//Read the tilemap that stores the positions in the tileset
		//Used to construct the map as a vertex_array appliying each texture to a quad
		if (node_name == "layer"  && object_name != "Collision")
		{
			pugi::xml_node node_data = node.child("data");
			std::string node_data_name = node_data.name();

			if (object_name == "Walls")
			{
				if (node_data_name == "data")
				{
					for (pugi::xml_node node_map = node_data.first_child(); node_map; node_map = node_map.next_sibling())
					{
						tmx_info.maps[tmx_info.num_layers].push_back(node_map.attribute("gid").as_int());
					}
				}
			}
			tmx_info.num_layers++;
		}

		if (node_name == "layer"  && object_name == "Collision")
		{
			pugi::xml_node node_data = node.child("data");
			std::string node_data_name = node_data.name();

			if (node_data_name == "data")
			{
				for (pugi::xml_node node_map = node_data.first_child(); node_map; node_map = node_map.next_sibling())
				{
					TMX_Tile tile;
					tile.type = static_cast<TileType>(node_map.attribute("gid").as_int());
					tmx_info.collision_map.push_back(tile);
				}
			}
		}

		//Read dots position
		if (node_name == "objectgroup")
		{
			pugi::xml_node node_data = node;
		

			if (object_name == "Small dot")
			{
				for (pugi::xml_node node_itr = node_data.first_child(); node_itr; node_itr = node_itr.next_sibling())
				{
					float x = node_itr.attribute("x").as_float();
					float y = node_itr.attribute("y").as_float();
					object_group.small_dots_positions.push_back(sf::Vector2f(x, y));
				}	
			}
			else if(object_name == "Big dot")
			{
				for (pugi::xml_node node_itr = node_data.first_child(); node_itr; node_itr = node_itr.next_sibling())
				{
					float x = node_itr.attribute("x").as_float();
					float y = node_itr.attribute("y").as_float();
					object_group.big_dots_positions.push_back(sf::Vector2f(x, y));
				}
			}
			if (object_name == "Warp")
			{
				//Read rectangle rect
				for (pugi::xml_node node_itr = node_data.first_child(); node_itr; node_itr = node_itr.next_sibling())
				{
					float left = node_itr.attribute("x").as_float();
					float top = node_itr.attribute("y").as_float();
					float width= node_itr.attribute("width").as_float();
					float height = node_itr.attribute("height").as_float();
					object_group.warp_rects.push_back(sf::FloatRect(left, top, width, height));

					pugi::xml_node node_properties = node_itr.child("properties");

					float x = -1, y = -1;

					//Read warp destiny as a property
					for (pugi::xml_node node_itr2 = node_properties.first_child(); node_itr2; node_itr2 = node_itr2.next_sibling())
					{
						std::string  property_name = node_itr2.attribute("name").as_string();

						
						if (property_name == "x") x = node_itr2.attribute("value").as_float();
						if (property_name == "y") y = node_itr2.attribute("value").as_float();

						if (x != -1 && y != -1)
						{
							object_group.warp_destinies.push_back(sf::Vector2f(x, y));
							x = y = -1;	
						}
						
					}
				}
			}
		}
	}
}

//Second load to define all the attributtes after reading the tmx file
void Map::load_collision_map()
{
	int i = 0;
	int max_columns = tmx_info.map_size.x;

	for (auto &itr : tmx_info.collision_map)
	{
		int row = i / max_columns;
		int column = i % max_columns;

		//Set position, size and center of the tile
		itr.rect.width = tmx_info.tile_size.x;
		itr.rect.height = tmx_info.tile_size.y;
		itr.rect.top = row * itr.rect.height;
		itr.rect.left = column * itr.rect.width;
		itr.center.x = itr.rect.left + itr.rect.width / 2.f;
		itr.center.y = itr.rect.top + itr.rect.height / 2.f;

		itr.change_direction_rect.width = 2.f;
		itr.change_direction_rect.height = 2.f;
		itr.change_direction_rect.left = itr.rect.left + (itr.rect.width / 2.f) - 1.f;
		itr.change_direction_rect.top = itr.rect.top + (itr.rect.height / 2.f) - 1.f;


		//Calcule adjacent tiles
		if (i + 1 >= 0) itr.adjacent_tiles.push_back(i + 1);
		if (i - 1 >= 0) itr.adjacent_tiles.push_back(i - 1);
		if (i + max_columns >= 0) itr.adjacent_tiles.push_back(i + max_columns);
		if (i - max_columns >= 0) itr.adjacent_tiles.push_back(i - max_columns);
		if (i + max_columns - 1 >= 0) itr.adjacent_tiles.push_back(i + max_columns - 1);
		if (i + max_columns + 1 >= 0) itr.adjacent_tiles.push_back(i + max_columns + 1);
		if (i - max_columns - 1 >= 0) itr.adjacent_tiles.push_back(i - max_columns - 1);
		if (i - max_columns + 1 >= 0)  itr.adjacent_tiles.push_back(i - max_columns + 1);

		itr.id = i;
		++i;
	}

	
	/*Layer layer;
	layer.entity_vertex.setPrimitiveType(sf::Quads);
	layer.entity_vertex.resize(tmx_info.map_size.x * tmx_info.map_size.y * 4); // Each tile have 4 vertex
	layer.layer_id = 1;
	
	sf::Vector2f TS(12.f, 12.f); //TILE_SIZE

	std::vector<int> map = tmx_info.maps[layer.layer_id];

	for (unsigned int i = 0; i < tmx_info.map_size.y; ++i)
	{
		for (unsigned int j = 0; j < tmx_info.map_size.x; ++j)
		{

			unsigned int tile_pos = j + i*tmx_info.map_size.x;
			unsigned int tile_texture_num = map[tile_pos];  //Tiled begins with number 1
			unsigned int TR = tile_texture_num / (layer.texture.getSize().x / static_cast<int>(TS.x)); //TILE_ROW
			unsigned int TC = tile_texture_num % (layer.texture.getSize().x / static_cast<int>(TS.x)); //TILE_COLUMN

			sf::Vertex* quad = &layer.entity_vertex[tile_pos * 4];

			quad[0].position = sf::Vector2f(j * TS.x, i * TS.y);
			quad[1].position = sf::Vector2f((j + 1) * TS.x, i * TS.y);
			quad[2].position = sf::Vector2f((j + 1) * TS.x, (i + 1) * TS.y);
			quad[3].position = sf::Vector2f(j * TS.x, (i + 1) * TS.y);


			if (tmx_info.collision_map[tile_pos].type == TileType::Collidable)
			{
				quad[0].color = sf::Color(255, 0, 0,120);
				quad[1].color = sf::Color(255, 0, 0, 120);
				quad[2].color = sf::Color(255, 0, 0, 120);
				quad[3].color = sf::Color(255, 0, 0, 120);	
			}
			else if (tmx_info.collision_map[tile_pos].type == TileType::Walkable)
			{
				quad[0].color = sf::Color(0, 255, 0, 120);
				quad[1].color = sf::Color(0, 255, 0, 120);
				quad[2].color = sf::Color(0, 255, 0, 120);
				quad[3].color = sf::Color(0, 255, 0, 120);
			}	
			else
			{
				quad[0].color = sf::Color(0, 0, 255, 60);
				quad[1].color = sf::Color(0, 0, 255, 60);
				quad[2].color = sf::Color(0, 0, 255, 60);
				quad[3].color = sf::Color(0, 0, 255, 60);
			}
		}
	}
	tmx_info.num_layers++;
	map_layers.push_back(layer);*/
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
	std::vector<int> map = all_map[layer_id];
	
	Layer layer;
	layer.texture.loadFromFile(spritesheet);
	layer.entity_vertex.setPrimitiveType(sf::Quads);
	layer.entity_vertex.resize(width * height * 4); // Each tile have 4 vertex
	layer.layer_id = layer_id;

	unsigned int collision_layer_correction = 5;

	sf::Vector2f TS(12.f, 12.f); //TILE_SIZE
	
	for (unsigned int i = 0; i < height; ++i)
	{
		for (unsigned int j = 0; j < width; ++j)
		{
			unsigned int tile_pos = j + i*width;
			unsigned int tile_texture_num = map[tile_pos] - collision_layer_correction;  //Tiled begins with number 1
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
	if (isActive())
	for (int i = 0; i < tmx_info.num_layers; ++i)
	{
		draw_layer(target, states, i);
	}
}

void Map::update(sf::Time dt)
{
	if (isTemporalDisabled == true)
	{
		time_disabled -= dt;
	}

	if (time_disabled <= sf::Time::Zero)
	{
		isTemporalDisabled = false;
		enable();
	}

}

void Map::scaleMap(sf::Vector2f &factor)
{
	scale(factor);

	//Scale rectangle used for collision detection
	for (auto &itr : tmx_info.collision_map)
	{
		itr.rect.width  *= factor.x;
		itr.rect.height *= factor.y;
		itr.rect.left   *= factor.x;
		itr.rect.top    *= factor.y;

		itr.change_direction_rect.width *= factor.x;
		itr.change_direction_rect.height *= factor.y;
		itr.change_direction_rect.left *= factor.x;
		itr.change_direction_rect.top *= factor.y;
	}
}

void Map::setMapPosition(sf::Vector2f pos)
{
	setPosition(pos);

	for (auto &itr : tmx_info.collision_map)
	{
		itr.rect.left += pos.x;
		itr.rect.top  += pos.y;

		itr.change_direction_rect.left += pos.x;
		itr.change_direction_rect.top += pos.y;
	} 
}

//The center of each tile defined by a rectangle. 
//When a character collides with the rect decides the possible directions 
sf::FloatRect Map::getChangeDirectionRect(int pos)
{
	sf::FloatRect rect = sf::FloatRect(tmx_info.collision_map[pos].rect.left + tmx_info.collision_map[pos].rect.width/2.f - 1.f,
									   tmx_info.collision_map[pos].rect.top + tmx_info.collision_map[pos].rect.height/2.f - 1.f, 
									   2.f, 
									   2.f);
	return rect;
}





