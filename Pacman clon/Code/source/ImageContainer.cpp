#include "..\header\ImageContainer.h"


ImageContainer::ImageContainer(const ContainerType c_type)
: separation(1.1f)
, mode(ContainerMode::None)
, type(c_type)
{
}


ImageContainer::~ImageContainer()
{
}


unsigned int ImageContainer::getCategory() const
{ 
	if (type == ContainerType::Lifes) return Category::LifeContainer; 
	else if (type == ContainerType::Pickups) return Category::PickupContainer;

	return Category::Other;
}


void ImageContainer::pushBack(const Textures::Type type, const TextureGenerator &tg)
{

	std::unique_ptr<Image> image(new Image(type, tg));

	if (mode == ContainerMode::Horizontal)
	{
		image->setPosition(sf::Vector2f(image_container.size() * (image->getRect().width * separation), 0.f));
	}

	image_container.push_back(image.get());
	addChild(std::move(image));
}

void ImageContainer::pushFront(const Textures::Type type, const TextureGenerator &tg)
{

	std::unique_ptr<Image> image(new Image(type, tg));

	if (image_container.size() >= 7)
	{
		popBack();

		for (auto &itr : image_container)
		{
			itr->setPosition(sf::Vector2f(itr->getPosition().x + image->getRect().width * separation, 0));
		}
	}

	if (mode == ContainerMode::Horizontal)
	{
		float width = 0;

		if (image_container.size() != 0) 
		    width = image_container.size() * (image->getRect().width * separation);
		
		image->setPosition(sf::Vector2f(-width , 0.f));
	}


	image_container.push_front(image.get());
	this->addChild(std::move(image));
}

void ImageContainer::popBack()
{
	if (!image_container.empty())
	{
		image_container.back()->removeFromScene();
		image_container.pop_back();
	}
}




