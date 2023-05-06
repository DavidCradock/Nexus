#pragma once
#include "../precompiled_header.h"
#include "../core/singleton.h"
#include "applicationManager.h"
#include "archiveManager.h"
#include "audioManager.h"
#include "guiManager.h"
#include "inputDeviceManager.h"
#include "renderTextureManager.h"
#include "shaderManager.h"
#include "spriteManager.h"
#include "textFontManager.h"
#include "textureManager.h"

namespace Nexus
{
	// Holds pointers to each of the various managers
	class Managers : public Singleton<Managers>
	{
	public:
		// Obtains pointers to each of the managers
		Managers();

		ApplicationManager* applications;
		ArchiveManager* archives;
		AudioManager* audio;
		GUIManager* gui;
		InputDeviceManager* input;
		RenderTextureManager* renderTextures;
		ShaderManager* shaders;
		SpriteManager* sprites;
		TextFontManager* textFonts;
		TextureManager* textures;
	};

	
}