#pragma once

#include "SceneNode.h"
#include "Image.h"

#include <list>

enum class ContainerType
{
	Lifes,
	Pickups
};

enum class ContainerMode
{
	None,
	Horizontal,
	Vertical
};

class ImageContainer : public SceneNode
{

	public:
		explicit ImageContainer(const ContainerType c_type);
		virtual ~ImageContainer();

		void pushBack(const Textures::Type type, const TextureGenerator &tg);
		void pushFront(const Textures::Type type, const TextureGenerator &tg);
		void popBack();
		virtual unsigned int getCategory() const;

		inline void setContainerMode(ContainerMode c_mode){ mode = c_mode; }

	private:
		std::list<Image*> image_container;
		float separation;
		ContainerMode mode;
		ContainerType type;
};

