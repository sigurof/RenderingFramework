#ifndef TEXTUREDMODEL_H
#define TEXTUREDMODEL_H
#pragma once
#include "MeshIdentifier.h"
#include "ModelTexture.h"



namespace ML {
	class TexturedModel
	{
	public:
		TexturedModel() {}
		TexturedModel(MeshIdentifier model, ModelTexture texture) : rawModel(model), texture(texture) { }

		MeshIdentifier getRawModel() const { return rawModel; }
		ModelTexture getTexture() { return texture; }

	private:
		MeshIdentifier rawModel;
		ModelTexture texture;
	};

	inline bool operator==(const ML::TexturedModel& lhs, const ML::TexturedModel& rhs) { return lhs.getRawModel().getVertexCount() == rhs.getRawModel().getVertexCount(); }
	inline bool operator!=(const ML::TexturedModel& lhs, const ML::TexturedModel& rhs) { return !operator==(lhs, rhs); }
	inline bool operator< (const ML::TexturedModel& lhs, const ML::TexturedModel& rhs) { return lhs.getRawModel().getVertexCount() < rhs.getRawModel().getVertexCount(); }
	inline bool operator> (const ML::TexturedModel& lhs, const ML::TexturedModel& rhs) { return  operator< (rhs, lhs); }
	inline bool operator<=(const ML::TexturedModel& lhs, const ML::TexturedModel& rhs) { return !operator> (lhs, rhs); }
	inline bool operator>=(const ML::TexturedModel& lhs, const ML::TexturedModel& rhs) { return !operator< (lhs, rhs); }
}






#endif //TEXTUREDMODEL_H

