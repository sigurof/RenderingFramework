#ifndef TEXTURE_H
#define TEXTURE_H
#pragma once/*
#include <string>*/



namespace ML {

	class Texture
	{
	public:
		Texture(const std::string& path, ImageFormatEnum format) : path(path), format(format) {
			switch (format)
			{
			case JPG:
				data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
				break;
			case PNG:
				data = stbi_load(path.c_str(), &width, &height, &nrChannels, STBI_rgb);
				break;
			case PNG_ALPHA:
				data = stbi_load(path.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
				break;
			default:
				throw(std::exception("ERROR! Could not load textue file."));
				break;
			}

		};

		void activate() {
			glGenTextures(1, &ID);

			glBindTexture(GL_TEXTURE_2D, ID);
			switch (format)
			{
			case PNG_ALPHA:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				break;
			default:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				break;
			}
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		unsigned int getID() { return ID; }

	private:
		unsigned int ID;
		unsigned char *data;
		int width;
		int height;
		int nrChannels;
		ImageFormatEnum format;

		std::string path;

		static unsigned int* texturesCount;
	};
};

#endif //TEXTURE_H

