#ifndef MESHINFO
#define MESHINFO
#pragma once

namespace ML{

	enum StorageEnum
	{
		Disc, Memory, None
	};

	class MeshInfo
	{
	public:
		/* Creates invalid instance */
		MeshInfo() : storage(None), memoryData(nullptr), memoryName(""), discLocation("") {  }

		MeshInfo(const std::shared_ptr<VertexData> md, std::string mn) : storage(StorageEnum::Memory), memoryData(md), memoryName(mn)
		{
			if (mn == "" || md == nullptr)
			{	/* Invalidate instance */
				storage = None;
				memoryName = "";
				memoryData = nullptr;
			}
		}
		
		MeshInfo(const std::string dl) : storage(StorageEnum::Disc), discLocation(dl) 
		{
			if (dl == "")
			{	/* Invalidate instance */
				storage = None;
			}
		}

		StorageEnum getStorage() const { return storage; }
		const std::string& getDiscLocation() const { return discLocation; }
		const std::shared_ptr<VertexData> getMemoryData() const { return memoryData; }
		//unsigned int getID() const { return ID; }
		const std::string& getMemoryName() const { return memoryName; }


		void setStorage(const StorageEnum sto) 
		{
			try
			{
				switch (sto)
				{
				case Disc:
					if (discLocation == "")
					{
						throw(std::exception("ERROR! Cannot set storage to Disc before defining discLocation\n"));
					}
					break;
				case Memory:
					if (!memoryData.operator bool() && memoryName=="")
					{
						throw(std::exception("ERROR! Cannot set storage to Memory before defining memoryData and memoryName\n"));
					}
					break;
				}
				storage = sto;
			}
			catch (const std::exception& err)
			{
				std::cout << "In MeshInfo::setStorage\n";
				throw err;
			}
		}
		void setDiscLocation(const std::string dis) { discLocation = dis; }
		void setMemoryData(const std::shared_ptr<VertexData> mem) { memoryData = mem; }

		explicit operator bool() const
		{
			return storage != None;
		}

	private:
		
		StorageEnum storage;
		std::string memoryName;
		std::string discLocation;
		std::shared_ptr<VertexData> memoryData;

		//static unsigned int nInstances;
		//unsigned int ID;

	};


	struct MeshInfoHasher
	{
		std::size_t operator()(const MeshInfo& meshInfo) const
		{
			std::size_t seed = 0;
			boost::hash_combine(seed, meshInfo.getStorage());
			boost::hash_combine(seed, meshInfo.getDiscLocation());
			boost::hash_combine(seed, meshInfo.getMemoryName());
			return seed;
		}
	};

	bool operator==(const ML::MeshInfo& lhs, const ML::MeshInfo& rhs)
	{
		//if (lhs.getStorage() == rhs.getStorage())
		//{
		//	if (lhs.getStorage() == Disc)
		//	{
		//		return lhs.getDiscLocation() == rhs.getDiscLocation();
		//	}
		//	else// == Memory
		//	{
		//		return lhs.getMemoryName() == rhs.getMemoryName();
		//	}
		//}
		//return false;
		return lhs.getStorage() == rhs.getStorage() && lhs.getMemoryName() == rhs.getMemoryName() && lhs.getDiscLocation() == rhs.getDiscLocation();
	}

	//inline bool operator!=(const ML::MeshInfo& lhs, const ML::MeshInfo& rhs) { return !operator==(lhs, rhs); }

	bool operator<(const ML::MeshInfo& lhs, const ML::MeshInfo& rhs)
	{
		if (lhs.getStorage() == rhs.getStorage())
		{
			if (lhs.getDiscLocation() == rhs.getDiscLocation())
			{
				if (lhs.getMemoryName() == rhs.getMemoryName())
				{
					return false;
				}
				return lhs.getMemoryName() < rhs.getMemoryName();
			}
			return lhs.getDiscLocation() < rhs.getDiscLocation();
		}
		return lhs.getStorage() < rhs.getStorage();
	}

	//inline bool operator> (const ML::MeshInfo& lhs, const ML::MeshInfo& rhs) { return operator< (rhs, lhs); }
	//inline bool operator<=(const ML::MeshInfo& lhs, const ML::MeshInfo& rhs) { return !operator> (lhs, rhs); }
	//inline bool operator>=(const ML::MeshInfo& lhs, const ML::MeshInfo& rhs) { return !operator< (lhs, rhs); }

}

#endif // !MESHINFO


#pragma once
