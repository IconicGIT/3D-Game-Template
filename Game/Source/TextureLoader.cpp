#define STB_IMAGE_IMPLEMENTATION
#include "Stb/stb_image.h"
#include "TextureLoader.h"
#include "Log.h"

Texture LoadTexture(std::string file_path)
{
	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	// load and generate the texture
	unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &nrChannels, 0);
	/*for (size_t i = 0; i < width * height * 3; i++)
	{
		DEBUG_LOG("%i ", data[i]);
	}*/
	if (data)
	{
		if (nrChannels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		if (nrChannels == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture " << file_path << std::endl;
	}

	stbi_image_free(data);
	
	Texture tex;
	tex.id = texture;
	tex.path = file_path;
	tex.type = "null";
	tex.width = width;
	tex.height = height;
	tex.nrChannels = nrChannels;


return tex;
}


std::vector<Texture> LoadSlicedTexture(std::string file_path, int horizontalAmount, int verticalAmount, int outputAmount)
{

	if (outputAmount > (verticalAmount * horizontalAmount)) 
		outputAmount = verticalAmount * horizontalAmount;

	std::vector<Texture> output;
	unsigned int texture;

	int currentAmount = 0;

	for (size_t v = 0; v < verticalAmount; v++)
	{
		for (size_t h = 0; h < horizontalAmount; h++)
		{
			if (currentAmount < outputAmount)
			{
				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D, texture);

				// set the texture wrapping parameters
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				// set texture filtering parameters
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				// load image, create texture and generate mipmaps
				int width, height, nrChannels;
				stbi_set_flip_vertically_on_load(true);

				// load and generate the texture
				unsigned char* data = stbi_load(file_path.c_str(), &width, &height, &nrChannels, 0);

				int sectionX = int(v * width / horizontalAmount);
				int sectionY = int(h * height / verticalAmount);
				int sectionWidth = width / horizontalAmount;
				int sectionHeight = height / verticalAmount;

					if (data)
					{
						if (nrChannels == 3)
						{
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sectionWidth, sectionHeight * horizontalAmount, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
							glTexSubImage2D(GL_TEXTURE_2D, 0, sectionX, sectionY, sectionWidth, sectionHeight * horizontalAmount, GL_RGB, GL_UNSIGNED_BYTE, data);
						}
						if (nrChannels == 4)
						{
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sectionWidth, sectionHeight * horizontalAmount, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
							glTexSubImage2D(GL_TEXTURE_2D, 0, sectionX, sectionY, sectionWidth, sectionHeight * horizontalAmount, GL_RGBA, GL_UNSIGNED_BYTE, data);
						}

						glGenerateMipmap(GL_TEXTURE_2D);
					}
					else
					{
						std::cout << "Failed to load texture " << file_path << std::endl;
					}

				stbi_image_free(data);

				Texture tex;
				tex.id = texture;
				tex.path = file_path;
				tex.type = "null";
				tex.width = sectionWidth;
				tex.height = sectionHeight;
				tex.nrChannels = nrChannels;

				int w, h;

				glBindTexture(GL_TEXTURE_2D, texture);
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
				glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);

				DEBUG_LOG("Width: %i", w);
				DEBUG_LOG("Height: %i", h);


				output.push_back(tex);
			}
			else
			{
				break;
			}
			currentAmount++;
		}
	}


	return output;
}


//int dataLength = height * width * nrChannels;


		//int sectionSizeH = int(width / horizontalAmount) * nrChannels;
		//int sectionSizeV = int(height / verticalAmount) * nrChannels;

		//int dataRow = width * nrChannels;

		//int strideUntilNextRow = dataRow - sectionSizeH;

		//int sectionDataSize = int(width / horizontalAmount) * int(height / verticalAmount) * nrChannels;
		//unsigned char* sectionData = new unsigned char[sectionDataSize];

		//int sectionDataIndex = 0;
		//int sectionHeightIndex = 0;



		////generate the data of the Nth section's texture data

		////start with the offset of the current section until the end of the data
		////add a row of the data to be in the same pixel x, but the next pixel y
		//for (size_t j = i * sectionSizeH; j < dataLength; j += strideUntilNextRow)
		//{
		//	//making sure that it only adds the necessary vertical data of the section
		//	if (sectionHeightIndex < int(sectionSizeV / nrChannels))
		//	{
		//		//start at the first pixel of the offseted data until the end of the current sliced data row
		//		for (size_t k = j; k < j + sectionSizeH; k++, sectionDataIndex++)
		//		{
		//			sectionData[sectionDataIndex] = data[k];
		//			//DEBUG_LOG("%i", sectionData[sectionDataIndex]);

		//		}
		//	}
		//	else
		//	{
		//		break;
		//	}
		//	sectionHeightIndex++;

		//}

		////unsigned char miniData[] = { 255,255,0,0,255,255,0,0,255,255,0,0 };

		///*for (size_t i = 0; i < width * height * nrChannels; i++)
		//{
		//	DEBUG_LOG("%i ", data[i]);
		//}*/
