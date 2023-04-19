#pragma once
#include "../precompiled_header.h"
#include "../core/singleton.h"

namespace Nexus
{
	enum TextureFiltering
	{
		nearest,
		linear,
		mipmaps
	};

	// A single texture
	class Texture
	{
		friend class ManagerTextures;

	public:
		Texture(const std::string& imageFilenameUsed, bool bImageFlipOnLoadIN = false, TextureFiltering filter = mipmaps);
		bool getLoaded(void);
		void load(void);
		void unload(void);

		void bind(void);

		unsigned int getID(void);

		// Returns width of image used for the texture.
		// If the texture isn't loaded yet, this uses the faster loadInfo() method of Image to obtain width and stores in imageWidth
		int getWidth(void);

		// If the texture isn't loaded yet, this uses the faster loadInfo() method of Image to obtain height and stores in imageHeight
		int getHeight(void);
	private:
		bool loaded;
		unsigned int refCount;		// Used by the manager 
		std::string imageFilename;	// Given during constructor, is the name of the filename which contains this image data
		unsigned int textureID;		// OpenGL texture ID
		bool bImageFlipOnLoad;
		int imageWidth;
		int imageHeight;
		TextureFiltering textureFilter;
	};
}