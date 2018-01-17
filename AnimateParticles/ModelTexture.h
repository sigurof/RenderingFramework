#ifndef MODELTEXTURE_H
#define MODELTEXTURE_H
#pragma once



namespace ML {
	class ModelTexture
	{
	public:
		ModelTexture(){}

		ModelTexture(int ID) : textureID(ID){}

		unsigned int getID() const { return textureID; }

	private:
		unsigned int textureID;

	};
}

#endif //MODELTEXTURE_H

