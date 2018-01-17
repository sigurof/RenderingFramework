#ifndef MODEL_H
#define MODEL_H
#pragma once


namespace ML {

	class Model
	{
	public:
		Model() {}

		Model(const Model& m) : rawModel(m.getRawModel()), drawMode(m.getDrawMode()), surface(m.getSurface()) {}

		Model(std::shared_ptr<MeshIdentifier> mesh, const ModelTexture& texture, GLenum drawMode, float damper, float refl) : rawModel(mesh), surface(texture, damper, refl), drawMode(drawMode) {}
		Model(std::shared_ptr<MeshIdentifier> mesh, const Color& color, GLenum drawMode, float damper, float refl) :
			rawModel(mesh), surface(color, damper, refl), drawMode(drawMode){}
		
		Model(std::shared_ptr<MeshIdentifier> mesh, const ModelTexture& texture, GLenum drawMode) : Model(mesh, texture, drawMode, 1 / 2, 1 / 2) {}
		Model(std::shared_ptr<MeshIdentifier> mesh, const Color& color, GLenum drawMode) : Model(mesh, color, drawMode, 1 / 2, 1 / 2) {}
		
		Model(std::shared_ptr<MeshIdentifier> mesh, const ModelTexture& texture) : Model(mesh, texture, GL_TRIANGLES) { }
		Model(std::shared_ptr<MeshIdentifier> mesh, const Color& color) : Model(mesh, color, GL_TRIANGLES) {}

		std::shared_ptr<MeshIdentifier> getRawModel() const { return rawModel; }
		
		const Surface& getSurface() const { return surface; }
		GLenum getDrawMode() const { return drawMode; }

		void setDrawMode(const GLenum mode) { drawMode = mode; }
		void setSurfaceShineVariables(float damper, float refl) {
			this->surface.setShineDamper(damper);
			this->surface.setReflectivity(refl);
		}
		

	private:

		std::shared_ptr<MeshIdentifier> rawModel;
		GLenum drawMode;
		Surface surface;
	};

	inline bool operator==(const ML::Model& lhs, const ML::Model& rhs) { return lhs.getRawModel()->getVertexCount() == rhs.getRawModel()->getVertexCount(); }
	inline bool operator!=(const ML::Model& lhs, const ML::Model& rhs) { return !operator==(lhs, rhs); }
	inline bool operator< (const ML::Model& lhs, const ML::Model& rhs) { return lhs.getRawModel()->getVertexCount() < rhs.getRawModel()->getVertexCount(); }
	inline bool operator> (const ML::Model& lhs, const ML::Model& rhs) { return  operator< (rhs, lhs); }
	inline bool operator<=(const ML::Model& lhs, const ML::Model& rhs) { return !operator> (lhs, rhs); }
	inline bool operator>=(const ML::Model& lhs, const ML::Model& rhs) { return !operator< (lhs, rhs); }
}






#endif //MODEL_H

