#ifndef LINE_H
#define LINE_H
#pragma once

namespace ML {
	/*
	All Entities should be instances in the entities vector in a Scene instance because a Scene also holds tracks of the
	meshes and models, which can be shared between several entities. Making two instances of the same mesh is just uploading
	unnecessary information to the graphics card.
	*/
	class Line
	{
	public:
		Line() : useGlDrawElementRange(false), startElement(1), endElement(0) {}

		Line(const Model& mod, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale, const Color& col)
			: useGlDrawElementRange(false), color(col)
		{
			entity = std::shared_ptr<Entity>(new Entity(mod, pos, rot, scale));
		}

		bool shouldDraw() const { return entity->shouldDraw(); }
		bool shouldUseGlDrawElementRange() const { return this->useGlDrawElementRange; }
		
		int getStartElement() const 
		{
			return this->startElement; 
		}
		
		int getEndElement() const 
		{
			return this->endElement; 
		}
		const std::shared_ptr<Entity> getEntity() const 
		{
			try
			{
				if (this == nullptr)
				{
					throw std::exception("ERROR! Trying to access nullptr\n");
				}
				return entity;
			}
			catch (const std::exception& err)
			{
				std::cout << "In Line::getEntity\n";
				throw err;
			}
		}

		void setUseGlDrawElementRange(bool use)
		{
			try
			{
				if (startElement > endElement)
				{
					throw std::exception("Error! startElement > endElement");
				}
				this->useGlDrawElementRange = use;
			}
			catch (const std::exception& err)
			{
				std::cout << "In Line::setUseGlDrawElementRange\n";
				throw err;
			}
		}

		void setStartElement(int start)
		{
			try
			{
				if (start >= endElement)
				{
					throw(std::exception("ERROR! Invalid value. start must be < endElement"));
				}
				if (start < 0)
				{
					throw(std::exception("ERROR! start must be >= 0"));
				}
				startElement = start;
			}
			catch (const std::exception& err)
			{
				std::cout << "In Line::setStartElement\n";
				throw err;
			}
		}

		void setEndElement(int end)
		{
			try
			{
				if (end >= (int)this->entity->getModel().getRawModel()->getVertexCount())
				{
					throw(std::exception("ERROR! Invalid value. end must be < vertex count"));
				}
				if (end < startElement)
				{
					throw(std::exception("ERROR! end must be > startElement"));
				}
				endElement = end;
			}
			catch (const std::exception& err)
			{
				std::cout << "In Line::setEndElement\n";
				throw err;
			}
		}




	private:

		Color color;
		std::shared_ptr<Entity> entity;
		bool useGlDrawElementRange;
		int startElement;
		int endElement;
		//template<typename... Args>
		//std::function<void(Args...)> drawFunction;


	};

}

#endif // LINE_H
