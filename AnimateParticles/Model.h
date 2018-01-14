#ifndef MODEL_H
#define MODEL_H
#pragma once
//#include "MeshIdentifier.h"
//#include "ModelTexture.h"
//#include "Color.h"
//#include "Surface.h"
#include "MeshIdentifier.h"


namespace ML {

	class Model
	{
	public:
		Model() {}

		Model(const Model& m) : rawModel(m.getRawModel()), drawMode(m.getDrawMode()), surface(m.getSurface()) {}

		Model(const MeshIdentifier& type, const ModelTexture& texture, GLenum drawMode, float damper, float refl) : rawModel(type), surface(texture, damper, refl) , drawMode(drawMode) {}
		Model(const MeshIdentifier& type, const Color& color, GLenum drawMode, float damper, float refl) : rawModel(type), surface(color, damper, refl) , drawMode(drawMode) {}

		Model(const MeshIdentifier& type, const ModelTexture& texture, GLenum drawMode) : Model(type, texture, drawMode, 1 / 2, 1 / 2) {}
		Model(const MeshIdentifier& type, const Color& color, GLenum drawMode) : Model(type, color, drawMode, 1 / 2, 1 / 2) {}
		
		Model(const MeshIdentifier& type, const ModelTexture& texture) : Model(type, texture, GL_TRIANGLES) { }
		Model(const MeshIdentifier& type, const Color& color) : Model(type, color, GL_TRIANGLES) {}

		const MeshIdentifier& getRawModel() const { return rawModel; }
		const Surface& getSurface() const { return surface; }
		GLenum getDrawMode() const { return drawMode; }

		void setDrawMode(const GLenum mode) { drawMode = mode; }
		void setSurfaceShineVariables(float damper, float refl) {
			this->surface.setShineDamper(damper);
			this->surface.setReflectivity(refl);
		}
		

	private:
		MeshIdentifier rawModel;
		GLenum drawMode;
		Surface surface;
	};

	inline bool operator==(const ML::Model& lhs, const ML::Model& rhs) { return lhs.getRawModel().getVertexCount() == rhs.getRawModel().getVertexCount(); }
	inline bool operator!=(const ML::Model& lhs, const ML::Model& rhs) { return !operator==(lhs, rhs); }
	inline bool operator< (const ML::Model& lhs, const ML::Model& rhs) { return lhs.getRawModel().getVertexCount() < rhs.getRawModel().getVertexCount(); }
	inline bool operator> (const ML::Model& lhs, const ML::Model& rhs) { return  operator< (rhs, lhs); }
	inline bool operator<=(const ML::Model& lhs, const ML::Model& rhs) { return !operator> (lhs, rhs); }
	inline bool operator>=(const ML::Model& lhs, const ML::Model& rhs) { return !operator< (lhs, rhs); }
}






#endif //MODEL_H

