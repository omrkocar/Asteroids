#pragma once

class Component;

namespace sf
{
	class RenderWindow;
}

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void LoadTexture(const char* fileName);

protected:
	
	
};