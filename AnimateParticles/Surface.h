#ifndef SURFACE_H
#define SURFACE_H
#pragma once


namespace ML {



	class Surface
	{
	public:
		
		Surface() : color(nullptr), texture(nullptr), colorIsActive(false), textureIsActive(false) {}

		Surface(const Surface& s) :
			colorIsActive(s.isColorActive()), textureIsActive(s.isTextureActive()), 
			color(nullptr), texture(nullptr),
			textureFraction(s.getTextureFraction()), shineDamper(s.getShineDamper()), reflectivity(s.getReflectivity())
		{
			if (s.color != nullptr)
			{
				color = new Color(*s.getColor());
			}
			if (s.getTexture() != nullptr)
			{
				texture = new ModelTexture(*s.getTexture());
			}
		}

		Surface(const ModelTexture& tex) : Surface(tex, 1 / 2, 1 / 2) {}
		
		Surface(const Color& col) : Surface(col, 1 / 2, 1 / 2) {}

		Surface(const ModelTexture& tex, float damper, float refl) :
			color(nullptr), texture(new ModelTexture(tex)),
			shineDamper(damper), reflectivity(refl), textureFraction(1),
			colorIsActive(false), textureIsActive(true)
		{}

		Surface(const Color& col, float damper, float refl) :
			texture(nullptr), color(new Color(col)),
			shineDamper(damper), reflectivity(refl), textureFraction(0),
			colorIsActive(true), textureIsActive(false)
		{}

		Surface(const Color& col, const ModelTexture& tex) : Surface(col, tex, 1 / 2, 1 / 2, 1 / 2) {}

		Surface(const Color& col, const ModelTexture& tex, float damper, float refl, float texFrac) :
			color(new Color(col)), texture(new ModelTexture(tex)),
			shineDamper(damper), reflectivity(refl), textureFraction(texFrac),
			colorIsActive(true), textureIsActive(true)
		{}

		~Surface() {
			delete color; color = nullptr;
			delete texture; texture = nullptr;
		}

		bool isColorActive() const { return colorIsActive; }
		bool isTextureActive() const { return textureIsActive; }
		const Color* getColor() const { return color; }
		const ModelTexture* getTexture() const { return texture; }

		void setColorActive(bool active) { colorIsActive = active; }
		void setTextureActive(bool active) { textureIsActive = active; }
		void setColor(const Color& col) { color = new Color(col); }
		void setTexture(const ModelTexture& tex) { texture = new ModelTexture(tex); }

		float getReflectivity() const { return reflectivity; }
		float getShineDamper() const { return shineDamper; }
		float getTextureFraction() const { return textureFraction; }
		void setReflectivity(const float refl) { reflectivity = refl; }
		void setShineDamper(const float shDa) { shineDamper = shDa; }
		void setTextureFraction(const float texFrac) { textureFraction = texFrac; }


	private:
		bool colorIsActive;
		bool textureIsActive;
		Color* color;
		ModelTexture* texture;
		float textureFraction;
		float shineDamper;
		float reflectivity;
	};

}


#endif // !SURFACE_H
