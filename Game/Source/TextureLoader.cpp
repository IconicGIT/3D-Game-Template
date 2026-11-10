#define STB_IMAGE_IMPLEMENTATION
#include "Stb/stb_image.h"
#include "TextureLoader.h"
#include "Log.h"

Texture LoadTexture(const std::string& file_path)
{
	GLuint texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR
	);
	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		GL_LINEAR
	);
	
	int width = 0;
	int height = 0;
	int nrChannels = 0;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(
		file_path.c_str(),
		&width,
		&height,
		&nrChannels,
		0
	);

	if (data == nullptr)
	{
		DEBUG_LOG(
			"Failed to load texture: %s",
			file_path.c_str()
		);

		glDeleteTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, 0);

		return Texture{};
	}

	GLenum format = GL_RGB;

	if (nrChannels != 1 &&
		nrChannels != 3 &&
		nrChannels != 4)
	{
		DEBUG_LOG("Unsupported channel count: %i", nrChannels);

		stbi_image_free(data);
		glDeleteTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, 0);

		return Texture{};
	}

	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		format,
		width,
		height,
		0,
		format,
		GL_UNSIGNED_BYTE,
		data
	);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

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
							glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sectionWidth, sectionHeight * horizontalAmount, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
							glTexSubImage2D(GL_TEXTURE_2D, 0, sectionX, sectionY, sectionWidth, sectionHeight * horizontalAmount, GL_RGB, GL_UNSIGNED_BYTE, data);
							glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
						}
						if (nrChannels == 4)
						{
							glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sectionWidth, sectionHeight * horizontalAmount, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
							glTexSubImage2D(GL_TEXTURE_2D, 0, sectionX, sectionY, sectionWidth, sectionHeight * horizontalAmount, GL_RGBA, GL_UNSIGNED_BYTE, data);
							glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
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
