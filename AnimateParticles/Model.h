#ifndef MODEL_H
#define MODEL_H
#pragma once


namespace ML {

	class Model
	{
	public:
		Model() : rawModel(nullptr) {}

		Model(const Model& m) : rawModel(m.getRawModel()), drawMode(m.getDrawMode()), surface(m.getSurface()) {}

		Model(std::shared_ptr<VAOHandle> mesh, const ModelTexture& texture, GLenum drawMode, float damper, float refl) : rawModel(mesh), surface(texture, damper, refl), drawMode(drawMode) {}
		Model(std::shared_ptr<VAOHandle> mesh, const Color& color, GLenum drawMode, float damper, float refl) :
			rawModel(mesh), surface(color, damper, refl), drawMode(drawMode){}
		
		Model(std::shared_ptr<VAOHandle> mesh, const ModelTexture& texture, GLenum drawMode) : Model(mesh, texture, drawMode, 1 / 2, 1 / 2) {}
		Model(std::shared_ptr<VAOHandle> mesh, const Color& color, GLenum drawMode) : Model(mesh, color, drawMode, 1 / 2, 1 / 2) {}
		
		Model(std::shared_ptr<VAOHandle> mesh, const ModelTexture& texture) : Model(mesh, texture, GL_TRIANGLES) { }
		Model(std::shared_ptr<VAOHandle> mesh, const Color& color) : Model(mesh, color, GL_TRIANGLES) {}

		std::shared_ptr<VAOHandle> getRawModel() const
		{
			try
			{
				if (this == nullptr)
				{
					throw std::exception("ERROR! Trying to access nullptr\n");
				}
				return rawModel;
			}
			catch (const std::exception& err)
			{
				std::cout << "In Model::getRawModel\n";
				throw err;
			}
		}
		
		const Surface& getSurface() const { return surface; }
		GLenum getDrawMode() const 
		{
			try
			{
				if (this == nullptr)
				{
					throw std::exception("ERROR! Trying to access nullptr\n");
				}
				return drawMode;
			}
			catch (const std::exception& err)
			{
				std::cout << "In Model::getDrawMode\n";
				throw err;
			}
		}

		void setDrawMode(const GLenum mode) { drawMode = mode; }

		//void setSurfaceShineVariables(float damper, float refl) {
		//	this->surface.setShineDamper(damper);
		//	this->surface.setReflectivity(refl);
		//}
		
	private:

		std::shared_ptr<VAOHandle> rawModel;
		GLenum drawMode;
		Surface surface;
	};

	bool operator==(const ML::Model& lhs, const ML::Model& rhs) { return lhs.getRawModel()->getVertexCount() == rhs.getRawModel()->getVertexCount(); }
	bool operator!=(const ML::Model& lhs, const ML::Model& rhs) { return !operator==(lhs, rhs); }
	bool operator< (const ML::Model& lhs, const ML::Model& rhs) { return lhs.getRawModel()->getVertexCount() < rhs.getRawModel()->getVertexCount(); }
	bool operator> (const ML::Model& lhs, const ML::Model& rhs) { return  operator< (rhs, lhs); }
	bool operator<=(const ML::Model& lhs, const ML::Model& rhs) { return !operator> (lhs, rhs); }
	bool operator>=(const ML::Model& lhs, const ML::Model& rhs) { return !operator< (lhs, rhs); }
}






#endif //MODEL_H

