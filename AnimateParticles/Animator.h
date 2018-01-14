#ifndef ANIMATOR_H
#define ANIMATOR_H
#pragma once

#include "GraphicsEngine.h"
#include <fstream>
#include <sstream>
#include "MeshIdentifier.h"

using namespace ML;

class Animator
{
public:

	Animator() : Animator("pos") { }

	Animator(const std::string& _path) : Animator(_path, GraphicsEngine()) {}

	Animator(const std::string& _path, const GraphicsEngine& graphicsEngine) : path(_path), ge(graphicsEngine)
	{
		try
		{
			ge.openWindow(1920, 1080);
			load(_path);
			t = times[0];
			makeEntities();
			//ge.loadEntities(entities);

		}
		catch (const std::exception& err)
		{
			std::cout << "In Animator(const std::string& _path, GraphicsEngine& graphicsEngine):\n";
			throw(err);
		}
	}

	void setLightPosition(const glm::vec3& pos) {
		this->ge.setLightPosition(pos);
	}

	void load(const std::string& _path) {
		try
		{
			/* Set path */
			//this->path = _path;
			std::ifstream ifile(path);
			if (ifile.good())
			{
				loadHeader(ifile);
				checkSizeOfData();
				//nDynamicParticles = (unsigned int)(cols / 3);
				dynamicData = std::vector<std::vector<glm::vec3>>(nFrames, std::vector<glm::vec3>(nDynamicParticles, glm::vec3(0, 0, 0)));
				staticData = std::vector<glm::vec3>(nFrames, glm::vec3(0, 0, 0));
				times = std::vector<double>(nFrames, 0);
				loadDynamicData(ifile);
			}
			else
			{
				throw(std::exception("ERROR! Could not open file"));
			}
		}
		catch (const std::exception& err)
		{
			std::cout << "In Animator::load:\n";
			throw(err);
		}
	}

	void setCameraPosition(const glm::vec3& pos) {
		ge.setCameraPosition(pos);
	}

	void drawSingleFrame(unsigned int i_time)
	{
		try
		{
			while (ge.windowIsOpen())
			{
				ge.drawScene();
				ge.pollUserInput();
			}
			ge.cleanUp();
		}
		catch (const std::exception& err)
		{
			std::cout << "In Animator::drawSingleFrame():\n";
			throw err;
		}
	}

	void setFrameRate(unsigned int fps) {
		double spf = 1 / (double)fps;
		double currentSpf = times[1] - times[0];
		double scaleFactor = spf / currentSpf;
		for (unsigned int i = 0; i < times.size(); i++)
		{
			times[i] *= scaleFactor;
		}
	}

	void play()
	{
		try
		{
			double nextTime = times[1]; unsigned int i_time = 1;
			t = 0;
			while (ge.windowIsOpen())
			{
				timer.start();
				ge.drawScene();
				ge.pollUserInput();
				if ((t+=timer.elapsedS()) >= nextTime)
				{
					updateEntityPositions(i_time);
					timer.reset();
					if (i_time < nFrames-1)
					{
						nextTime = times[++i_time];
					}
				}
			}
			ge.cleanUp();
		}
		catch (const std::exception& err)
		{
			std::cout << "In Animator::play():\n";
			throw err;
		}

	}

	void hideBox() {
		ge.hideStaticEntity(0);
	}

private:

	void updateEntityPositions(unsigned int data_index) 
	{
		ge.shiftEntities(dynamicData[data_index]);
	}


	void makeEntities() {
		for (unsigned int i = 0; i < nDynamicParticles; i++)
		{
			ge.addEntity(dynamicTypes[i], Color(ColorEnum::BLUE) , dynamicData[0][i], dynamicAngles[i], dynamicScales[i], true);
		}
		for (unsigned int i = 0; i < nStaticParticles; i++)
		{
			ge.addEntity(staticTypes[i], Color(ColorEnum::YELLOW, 1.0), staticData[i], staticAngles[i], staticScales[i], false);
		}
		//for (unsigned int i = 0; i < nStaticParticles; i++)
		//{
		//	ge.addEntity(t)
		//}
	}

	void loadHeader(std::ifstream& ifile)
	{
		/*
		Format:

		0	nDynamicObjects		nStaticObjects		nFrames
		1	staticObject pos rot scale
		2	staticObject pos rot scale
		3	staticObj.................
		4	..........................
		5	dynamicObject rot scale 	dynamicObject rot scale 	dynamicObject rot scale
		6	position					position					position
		7	position					position					position
		8	position						........................................
		9	.........................................................................


		*/

		try
		{
			std::string dimsLine, staticLine, dynamicLine, colorsLine;
			std::vector<std::string> dimsStrings, radiiStrings, colorsStrings;
			std::string word;

			/* nDynamicObjects	nStaticObjects	nFrames */
			if (!std::getline(ifile, dimsLine)) { throw(std::exception("ERROR in Animator::loadHeader: Could not load data set dimensions from header\n")); }
			std::istringstream dimsIss(dimsLine);
			while (dimsIss >> word) { dimsStrings.push_back(word); }
			nDynamicParticles = std::stoi(dimsStrings[0]); nStaticParticles = std::stoi(dimsStrings[1]); nFrames = std::stoi(dimsStrings[2]);

			/* Read info on staticObjects */
			for (unsigned int i = 0; i < nStaticParticles; i++)
			{
				if (!std::getline(ifile, staticLine)) { throw(std::exception("ERROR in Animator::loadHeader: Could not load static data set from header\n")); }
				std::istringstream staticIss(staticLine);
				MeshEnum type; glm::vec3 pos; glm::vec3 rot; glm::vec3 scale;
				if (readType(staticIss, type)) { staticTypes.push_back(type);  }	
				if (readVec3(staticIss, pos)) { staticData.push_back(pos); }
				if (readVec3(staticIss, rot)) { staticAngles.push_back(rot); }
				if (readVec3(staticIss, scale)) { staticScales.push_back(scale); }
			}

			/* Read header for dynamicObjects */
			if (!std::getline(ifile, dynamicLine)) { throw(std::exception("ERROR! in Animator::loadHeader: Could not load dynamic header\n")); }
			std::istringstream dynamicIss(dynamicLine);
			MeshEnum dynType; glm::vec3 dynRot; glm::vec3 dynScale;
			for (unsigned int i = 0; i < nDynamicParticles; i++)
			{
				if (readType(dynamicIss, dynType)) { dynamicTypes.push_back(dynType); }
				if (readVec3(dynamicIss, dynRot))   { dynamicAngles.push_back(dynRot); }
				if (readVec3(dynamicIss, dynScale)) { dynamicScales.push_back(dynScale); }
			}
		}
		catch (const std::exception& err)
		{
			std::cout << "In Animator::loadHeader: \n";
			throw err;
		}
	}

	void loadDynamicData(std::ifstream& ifile) {
		try
		{
			glm::vec3 pos(0, 0, 0);
			std::string line;
			std::string word;
			int i(0); while (std::getline(ifile, line))
			{
				std::istringstream iss(line);
				iss >> word;
				times[i] = std::stod(word);
				int j(0); while (readVec3(iss, pos))
				{
					dynamicData[i][j] = pos;
					j++;
				}
				i++;
			}
		}
		catch (const std::exception& err)
		{
			std::cout << "In loadData:\n";
			throw(err);
		}
	}

	//void loadHeader(std::ifstream& ifile)
	//{
	//	/*
	//	6, 20
	//	Ball	Ball	//Type of object
	//	0.2	0.2 	// Specifiers for radius, side-lengths etc
	//	0 0 0	0 1 0	// Color
	//	*/
	//	try
	//	{
	//		std::string dimsLine, typesLine, radiiLine, colorsLine;
	//		std::vector<std::string> dimsStrings, radiiStrings, colorsStrings;
	//		std::string word;
	//		/* Find rows, cols */
	//		if (!std::getline(ifile, dimsLine)) { throw(std::exception("ERROR in Animator::loadHeader: Could not load data set dimensions from header\n")); }
	//		std::istringstream dimsIss(dimsLine);
	//		while (dimsIss >> word) { dimsStrings.push_back(word); }
	//		rows = std::stoi(dimsStrings[0]); cols = std::stoi(dimsStrings[1]);
	//		/* Find types of objects  */
	//		if (!std::getline(ifile, typesLine)) { throw(std::exception("ERROR in Animator::loadHeader: Could not load data set types from header\n")); }
	//		std::istringstream typesIss(typesLine);
	//		MeshEnum type;
	//		while (readType(typesIss, type)) { types.push_back(type); }
	//		/* Find radii */
	//		if (!std::getline(ifile, radiiLine)) { throw(std::exception("ERROR in Animator::loadHeader: Could not load data set radii from header\n")); }
	//		std::istringstream radiiIss(radiiLine);
	//		while (radiiIss >> word) { radii.push_back(stof(word)); }
	//		/* Find colors */
	//		if (!std::getline(ifile, colorsLine)) { throw(std::exception("ERROR in Animator::loadHeader: Could not load data set radii from header\n")); }
	//		std::istringstream colorsIss(colorsLine);
	//		glm::vec3 vec(0, 0, 0);
	//		while (readVec3(colorsIss, vec)) { colors.push_back(vec); }
	//	}
	//	catch (const std::exception& err)
	//	{
	//		std::cout << "In Animator::loadHeader: \n";
	//		throw err;
	//	}
	//}
	//void loadData(std::ifstream& ifile) {
	//	try
	//	{
	//		glm::vec3 pos(0, 0, 0);
	//		std::string line;
	//		std::string word;
	//		int i(0); while (std::getline(ifile, line))
	//		{
	//			std::istringstream iss(line);
	//			iss >> word;
	//			times[i] = std::stod(word);
	//			int j(0); while (readVec3(iss, pos))
	//			{
	//				data[i][j] = pos;
	//				j++;
	//			}
	//			i++;
	//		}
	//	}
	//	catch (const std::exception& err)
	//	{
	//		std::cout << "In loadData:\n";
	//		throw(err);
	//	}
	//}


	bool readType(std::istringstream& iss, MeshEnum& type) {
		try
		{

			std::string word;
			if (iss >> word)
			{
				if (word == "Ball") { type = MeshEnum::Ball; }
				else if (word == "Cube") { type = MeshEnum::Cube; }
				else { throw(std::exception("ERROR! Invalid type specified")); }
				return true;
			}
			return false;
		}
		catch (const std::exception& err)
		{
			std::cout << "In readType():\n";
			throw err;
		}
	}

	bool readVec3(std::istringstream& iss, glm::vec3& vec) {
		std::string word;
		for (unsigned int i = 0; i < 3; i++)
		{
			if (iss >> word)
			{
				vec[i] = std::stof(word);
			}
			else
			{
				if (i != 0)
				{
					throw("Error! Coordinate entries not a multiple of 3 (3 cartesian coords)");
				}
				return false;
			}
		}
		return true;
	}

	void checkSizeOfData() {
		try
		{
			unsigned int tooBigN = (unsigned int)(1000000000);
			unsigned int N = nFrames * nDynamicParticles * 3 * 12;
			N += nFrames * 24;
			if (N > tooBigN)
			{
				throw(std::exception("Error! Data set is too large\n"));
			}
		}
		catch (const std::exception& err)
		{
			std::cout << "In checkSizeOfData:\n";
			throw err;
		}
	}

	unsigned int nDynamicParticles, nStaticParticles, nFrames;
	
	std::vector<std::vector<glm::vec3>> dynamicData;
	std::vector<MeshEnum> dynamicTypes;
	std::vector<glm::vec3> dynamicAngles;
	std::vector<glm::vec3> dynamicScales;

	std::vector<glm::vec3> staticData;
	std::vector<MeshEnum> staticTypes;
	std::vector<glm::vec3> staticAngles;
	std::vector<glm::vec3> staticScales;

	std::string path;

	GraphicsEngine ge;

	//std::vector<Entity*> entities;

	std::vector<double> times;

	double t;

	Clock timer;
	//Loader loader;

};

#endif // !ANIMATOR_H
