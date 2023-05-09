#include "precompiled_header.h"
#include "texture.h"
#include "../../core/log.h"
#include "../../graphics/image.h"

namespace Nexus
{
	Texture::Texture(const std::string& imageFilenameUsed, bool bImageFlipOnLoadIN, TextureFiltering filter)
	{
		bImageFlipOnLoad = bImageFlipOnLoadIN;
		loaded = false;
		refCount = 0;
		imageFilename = imageFilenameUsed;
		textureID = 0;
		imageWidth = imageHeight = 0;
		textureFilter = filter;
	}

	bool Texture::getLoaded(void)
	{
		return loaded;
	}

	void Texture::load(void)
	{
		/*
		Wrapping
		GL_REPEAT: The default behavior for textures. Repeats the texture image.
		GL_MIRRORED_REPEAT: Same as GL_REPEAT but mirrors the image with each repeat.
		GL_CLAMP_TO_EDGE: Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.
		GL_CLAMP_TO_BORDER: Coordinates outside the range are now given a user-specified border color.

		Border colour
		float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		*/
		Image image;
		image.load(imageFilename, bImageFlipOnLoad);
		glGenTextures(1, &textureID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		// Set the texture wrapping/filtering options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (textureFilter == TextureFiltering::mipmaps)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else if (textureFilter == TextureFiltering::linear)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
		}
		else if (textureFilter == TextureFiltering::nearest)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getWidth(), image.getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getData());
		}
		imageWidth = image.getWidth();
		imageHeight = image.getHeight();
	}

	void Texture::unload(void)
	{

	}

	void Texture::bind(void)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	unsigned int Texture::getID(void)
	{
		return textureID;
	}

	int Texture::getWidth(void)
	{
		if (0 == imageWidth)
		{
			int numChan;
			Image img;
			img.loadInfo(imageFilename, imageWidth, imageHeight, numChan);
		}
		return imageWidth;
	}

	int Texture::getHeight(void)
	{
		if (0 == imageHeight)
		{
			int numChan;
			Image img;
			img.loadInfo(imageFilename, imageWidth, imageHeight, numChan);
		}
		return imageHeight;
	}
}