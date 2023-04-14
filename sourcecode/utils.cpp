#include "precompiled_header.h"
#include "utils.h"

namespace Nexus
{

	void getHueColour(float fHueAmount, float& fRed, float& fGreen, float& fBlue)
	{
		clamp(fHueAmount, 0.0f, 1.0f);
		fHueAmount *= 360.0f;
		if (fHueAmount <= 60.0f)	// Inc green
		{
			fRed = 1.0f;
			fGreen = fHueAmount / 60.0f;
			fBlue = 0.0f;
		}
		else if (fHueAmount <= 120.0f)	// Dec red
		{
			fRed = 1.0f - ((fHueAmount - 60.0f) / 60.0f);
			fGreen = 1.0f;
			fBlue = 0.0f;
		}
		else if (fHueAmount <= 180.0f)	// Inc blue
		{
			fRed = 0.0f;
			fGreen = 1.0f;
			fBlue = (fHueAmount - 120.0f) / 60.0f;
		}
		else if (fHueAmount <= 240.0f)	// Dec green
		{
			fRed = 0.0f;
			fGreen = 1.0f - ((fHueAmount - 180.0f) / 60.0f);
			fBlue = 1.0f;
		}
		else if (fHueAmount <= 300.0f)	// Inc red
		{
			fRed = (fHueAmount - 240.0f) / 60.0f;
			fGreen = 0.0f;
			fBlue = 1.0f;
		}
		else // dec blue
		{
			fRed = 1.0f;
			fGreen = 0.0f;
			fBlue = 1.0f - ((fHueAmount - 300.0f) / 60.0f);
		}
	}

	bool fileExists(const std::string& strFilename)
	{
		FILE* f;
		if (fopen_s(&f, strFilename.c_str(), "rb"))
			return false;
		fclose(f);
		return true;
	}

	bool convertFileToHeader(std::string strFilename, std::string strArrayName, unsigned int uiNumElementsPerRow)
	{
		FILE* fs = NULL;  // Source file
		FILE* fd = NULL;  // Destination file

		std::string strFilenameInput = strFilename;
		std::string strFilenameOutput = strFilename;
		strFilenameOutput.append(".h");

		// Open source
		fopen_s(&fs, strFilename.c_str(), "rb");
		if (fs == NULL)
			return false;

		// Open destination
		fopen_s(&fd, strFilenameOutput.c_str(), "wb");
		if (fd == NULL)
		{
			fclose(fs);
			return false;
		}

		// Write comment giving name of binary file that this data came from
		fprintf(fd, "// Following data taken from file \"%s\"\n", strFilename.c_str());

		// Get length of data
		fseek(fs, 0, SEEK_END);
		int length = ftell(fs);
		fseek(fs, 0, SEEK_SET);

		// Write #define for length of array
		std::string strArrayNameUppercase = strArrayName;
		std::transform(strArrayNameUppercase.begin(), strArrayNameUppercase.end(), strArrayNameUppercase.begin(), ::toupper);
		strArrayNameUppercase.append("_SIZE");
		fprintf(fd, "#define %s %d\n", strArrayNameUppercase.c_str(), length);

		// Write start of array
		fprintf(fd, "unsigned char %s[] =\n", strArrayName.c_str());
		fprintf(fd, "{\n ");

		// Write out data from source binary file
		while (feof(fs) == false)
		{
			unsigned char tmp;
			for (int e = 0; e < (int)uiNumElementsPerRow; e++)
			{
				fread(&tmp, 1, sizeof(unsigned char), fs);
				if (feof(fs) == false)
					fprintf(fd, "0x%02x,", tmp);
			}

			if (feof(fs)) // Reached end of file
			{
				fseek(fd, -1, SEEK_CUR);  // Remove last comma
				fprintf(fd, "};");
			}
			fprintf(fd, "\n ");
		}
		fprintf(fd, "\n");

		fclose(fs);
		fclose(fd);
		return true;
	}
}
