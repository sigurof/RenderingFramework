#ifndef ANIMATOR_H
#define ANIMATOR_H
#pragma once


using namespace ML;
template <class T>
using sptr = std::shared_ptr<T>;

class Animator
{
public:

	Animator(const std::string& path) : running(false)
	{
		nextOutputFrame = 0;
		try
		{
			load(path);
		}
		catch (const std::exception& err)
		{
			std::cout << "In Animator(const std::string& path, GraphicsEngine& graphicsEngine):\n";
			throw(err);
		}
	}

	Animator(const Animator&) = delete;

	Animator& operator=(const Animator&) = delete;

	sptr<Scene> makeScene(Loader& loader)
	{
		try
		{
			sptr<Scene> scene = sptr<Scene>(new Scene);
			for (unsigned int i = 0; i < nDynamicParticles; i++)
			{
#ifdef NOUPLOADING
				MeshInfo info;
				switch (dynamicTypes[i])
				{
				case Sphere:
					info.setDiscLocation(OBJECTPATH + "sphere.obj");
					info.setStorage(StorageEnum::Disc);
					break;
				case Cube:
					info.setDiscLocation(OBJECTPATH + "cube.obj");
					info.setStorage(StorageEnum::Disc);
				default:
					throw(std::exception("ERROR! Could not recognize dynamic type\n"));
					break;
				}
				scene->addSurface(info, Color(ColorEnum::ORANGE), dynamicData[0][i], dynamicAngles[i], dynamicScales[i], true);
#else
				scene->addSurface(dynamicTypes[i], Color(ColorEnum::ORANGE), dynamicData[0][i], dynamicAngles[i], dynamicScales[i], true);
#endif // NOUPLOADING
				animateEntities.push_back(scene->getSurfaces()[i]);
				assert(scene->getSurfaces()[i].use_count() == 2);
			}
			for (unsigned int i = 0; i < nStaticParticles; i++)
			{
#ifdef NOUPLOADING
				MeshInfo info;
				switch (staticTypes[i])
				{
				case Sphere:
					info.setDiscLocation(OBJECTPATH + "sphere.obj");
					info.setStorage(StorageEnum::Disc);
					break;
				case Cube:
					info.setDiscLocation(OBJECTPATH + "cube.obj");
					info.setStorage(StorageEnum::Disc);
					break;
				default:
					throw(std::exception("ERROR! Could not recognize object static type\n"));
					break;
				}
				scene->addSurface(info, Color(ColorEnum::GREEN, 0.4f), staticData[i], staticAngles[i], staticScales[i], true);
#else
				scene->addSurface(staticTypes[i], Color(ColorEnum::GREEN, 0.4f), staticData[i], staticAngles[i], staticScales[i], false);
#endif // NOUPLOADING
			}
			return scene;
		}
		catch (const std::exception& err)
		{
			std::cout << "In Animator::makeScene\n";
			throw err;
		}
	}

	//void addParticleTraceToScene(sptr<Scene> scene, unsigned int particleIndex, const Color& col)
	//{
	//	VAOHandle mesh()
	//	scene->addEntity(Line, col, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), false);
	//}

#ifdef NOUPLOADING
	void addParticleTraceToScene(std::shared_ptr<Scene> scene, unsigned int particleIndex)
	{
		std::shared_ptr<VertexData> data(new VertexData);
		std::vector<float> vertices = std::vector<float>(3 * dynamicData.size(), 0.0f);
		std::vector<unsigned int> indices = std::vector<unsigned int>(2 * dynamicData.size(), 0);
		for (size_t i = 0; i < dynamicData.size(); i++)
		{
			vertices[3 * i + 0] = dynamicData[i][particleIndex].x;
			vertices[3 * i + 1] = dynamicData[i][particleIndex].y;
			vertices[3 * i + 2] = dynamicData[i][particleIndex].z;
			indices[2 * i + 0] = i;
			indices[2 * i + 1] = i + 1;
			//indices[2 * i + 2] = i + 2;
		}
		indices[indices.size() - 1] = indices[0];
		data->setVertices(vertices);
		data->setIndices(indices);
		MeshInfo qualifier(data, std::string("particleTrace" + std::to_string(particleIndex)));
		scene->addLine(qualifier, Color(ColorEnum::WHITE), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), false);
	}
#endif // NOUPLOADING


	void load(const std::string& path)
	{
		try
		{
			/* Set path */
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

	void setFrameRate(unsigned int fps) {
		double spf = 1 / (double)fps;
		double currentSpf = times[1] - times[0];
		double scaleFactor = spf / currentSpf;
		for (unsigned int i = 0; i < times.size(); i++)
		{
			times[i] *= scaleFactor;
		}
	}

	void restart()
	{
		start(1);
		for (unsigned int i_entity = 0; i_entity < animateEntities.size(); i_entity++)
		{
			animateEntities[i_entity]->setPosition(dynamicData[0][i_entity]);
		}
	}

	float getNextFrameInterval() const { return nextFrameInterval; }

	void start(unsigned int startFrameIndex) {
		nextOutputFrame = startFrameIndex;
		i_frame = startFrameIndex;
		internalClock.start();
		nextFrameInterval = (float)(times[i_frame + 1] - times[i_frame]);// lastFrameTime;
	}

	const Clock& getInternalClock() { return internalClock; }

	void updateScene()
	{
		++i_frame;
		//if (i_frame == nextOutputFrame)
		//{
		//	std::cout << "\r"; std::cout << i_frame / (int)nFrames*100 << "%";
		//	nextOutputFrame = (unsigned int)((double)nFrames / 10 + i_frame);
		//}
		for (unsigned int i_entity = 0; i_entity < animateEntities.size(); i_entity++)
		{
			animateEntities[i_entity]->setPosition(dynamicData[i_frame][i_entity]);
		}
		if (i_frame + 1 < times.size() - 1)
		{
			nextFrameInterval = (float)(times[i_frame + 1] - times[i_frame]);
			internalClock.start();
		}
		else
		{
			//std::cout << "\n";
			// Program::run() will now always return false and ceises to run this function.
			nextFrameInterval = std::numeric_limits<float>::infinity();
		}
	}

	const Clock& getInternalClock() const { return internalClock; }

	void stop() {
		/* ... stop running commands*/
		running = false;
	}

	bool isRunning() const { return running; }

	void setRunning(bool run) { running = run; }

private:

	//void makeEntities(sptr<Scene> scene)
	//{
	//	for (unsigned int i = 0; i < nDynamicParticles; i++)
	//	{
	//		scene->addSurface(dynamicTypes[i], Color(ColorEnum::BLUE), dynamicData[0][i], dynamicAngles[i], dynamicScales[i], true);
	//		animateEntities.push_back(scene->getSurfaces()[i]);
	//		assert(scene->getSurfaces()[i].use_count() == 2);
	//	}
	//	for (unsigned int i = 0; i < nStaticParticles; i++)
	//	{
	//		scene->addSurface(staticTypes[i], Color(ColorEnum::YELLOW, 0.4f), staticData[i], staticAngles[i], staticScales[i], false);
	//	}
	//}

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
				if (readType(staticIss, type)) { staticTypes.push_back(type); }
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
				if (readVec3(dynamicIss, dynRot)) { dynamicAngles.push_back(dynRot); }
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

	bool readType(std::istringstream& iss, MeshEnum& type) {
		try
		{

			std::string word;
			if (iss >> word)
			{
				if (word == "Ball") { type = MeshEnum::Sphere; }
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

	std::vector<std::shared_ptr<Entity>> animateEntities;

	std::vector<double> times;
	double t;
	Clock internalClock;
	bool running;
	int i_frame;
	float nextFrameInterval;
	float lastFrameTime;
	unsigned int nextOutputFrame;

};

#endif // !ANIMATOR_H
