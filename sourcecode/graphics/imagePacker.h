#pragma once
#include "../precompiled_header.h"
#include "image.h"
#include "../math/vector2.h"

namespace Nexus
{

	// A utility class to pack many images into a single image efficiently
	// How to use...
	// // Create instance of this class
	// ImagePacker ip;
	// // Create/load all images that we wish to pack into a single image
	// Image img1;	img1.load("whatever.tga");
	// Image img2;	img2.load("whatever2.tga");
	// // ..........................................
	// Image img27;	img27.load("whatever27.tga");
	// // Now add them to this object, giving them a unique name
	// ip.addImage(img1, "image_name_01");
	// ip.addImage(img27, "image_name_27");
	// // Now call pack()
	// ip.pack();
	// // Now this object has an image containing all of the added images, packed into
	// // a single image, we can retrieve this image...
	// ip.getFinalImage().save("output_image_containing_packed_images.tga");
	// // We can also retrieve the positions of each of the added images in the final image
	// Vector2 vPos = ip.getImagePosition("image_name_01");
	// // We can also retrieve 0.0f to 1.0f texture coordinates for each named image
	// Vector2 vTC[2];
	// ip.getImageTextureCoords("image_name_01", vTC[0], vTC[1]);
	// Once we have finished we can simply destroy the ImagePacker object to free all temp mem.
	// If however, we wish to perform another image pack, then we can call reset() and start over.
	// Also note, that the image packer creates a copy of any image you add, so you can just use a
	// single Image object and parse that into the ImagePacker::add() method, reducing temporary memory
	// usage.\n
	// For example...
	// Image imgTemp;
	// ImagePacker ip;
	// imgTemp.load("img1.tga");
	// ip.addImage(imgTemp, "image 1");
	// imgTemp.load("img2.tga");
	// ip.addImage(imgTemp, "image 2");
	// // etc...
	class ImagePacker
	{
	public:
		ImagePacker();
		~ImagePacker();

		// Adds an image to be packed into the final image
		// Once the new image has been added, you can free if you no longer need it, as a copy is stored in here.
		// newImage An image that will be packed into the final image.
		// strID	A string which is used as an ID for the added image, which we use to identity the image when using the various get methods.
		// Returns false if the given unique image name already exists, else true. If false is returned, then you'll have to choose another name. 
		bool addImage(const Image& newImage, const std::string& strID);

		// Performs the image pack algorithm.
		// Once all the images which are to be packed are added with addImage(), call this to build the final image.
		// Once this has been called, you can use the various get methods.
		// bAllowRotation Whether to allow rotation of the source images so that they may fit better. 
		void pack(bool bAllowRotation = true);

		// Resets everything so we can start over
		void reset(void);

		// Retrieves the final image which contains all the packed images inside of it after addImage and pack have been called.
		// If no images have been added and/or pack() hasn't been called, this will return an empty image 
		Image& getFinalImage(void);

		// Retrieves the position within the final image of the given image ID
		// If no images have been added and/or pack() hasn't been called, this will return a zero vector
		// strID the ID given to the image when it was added with addImage()
		// Returns the position of the bottom left X and Y position of the image within the final image 
		Vector2 getImagePosition(const std::string& strID);

		// Retrieves the texture coordinates within the final image of the given image ID
		// If no images have been added and/or pack() hasn't been called, this will not touch the Vector2 params
		// \param strID the ID given to the image when it was added with addImage()
		// \param vTCBL a 2D vector holding bottom left x/y texture coordinates for the named image
		// \param vTCBR a 2D vector holding bottom right x/y texture coordinates for the named image
		// \param vTCTR a 2D vector holding top right x/y texture coordinates for the named image
		// \param vTCTL a 2D vector holding top left x/y texture coordinates for the named image 
		void getImageTextureCoords(const std::string& strID, Vector2& vTCBL, Vector2& vTCBR, Vector2& vTCTR, Vector2& vTCTL);
	private:
		Image imageFinal;	// Final image

		// Holds final data after pack() has been called
		struct ImagePackerImg
		{
			Image* pImage;			// An added image
			Vector2 position;		// A 2D vector holding an added image's bottom left position within the final image
			Vector2 TCBL;			// A 2D vector holding an added image's bottom left x/y texture coordinate
			Vector2 TCBR;			// A 2D vector holding an added image's bottom right x/y texture coordinate
			Vector2 TCTR;			// A 2D vector holding an added image's top right x/y texture coordinate
			Vector2 TCTL;			// A 2D vector holding an added image's top left x/y texture coordinate
		};
		std::map<std::string, ImagePackerImg*>	addedImages;	// A map holding each added image

		// Used by pack()
		void packInsertImg(Image* pImageSrc, unsigned int iIndex, int iPosX, int iPosY);	// Insert given image into final at specified position
		// Performs the actual packing into final image. Returns false if final image has to be bigger
		bool performPack(unsigned int iFinalImageDims);
		std::vector<Image*> image;
		std::vector<std::string> ID;
		std::vector<bool> rotated;
	};

}
