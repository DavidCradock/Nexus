#include "precompiled_header.h"
#include "geometryManager.h"
#include "../core/log.h"

namespace Nexus
{
	GeometryManager::GeometryManager()
	{
	}

	unsigned int GeometryManager::getNumGroups(void)
	{
		return (unsigned int)group.size();
	}

	unsigned int GeometryManager::getNumResInGroup(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("GeometryManager::getNumResInGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist!");
			Log::getPointer()->exception(err);
		}
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);
		return (unsigned int)itg->second->resource.size();;
	}

	unsigned int GeometryManager::getNumResInGroupLoaded(const std::string& strGroupName)
	{
		if (!groupExists(strGroupName))
		{
			std::string err("GeometryManager::getNumResInGroupLoaded(\"");
			err.append(strGroupName);
			err.append("\") failed. The group doesn't exist!");
			Log::getPointer()->exception(err);
		}
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);

		unsigned int iResLoadedTotal = 0;
		std::map<std::string, Geometry*>::iterator it = itg->second->resource.begin();
		while (it != itg->second->resource.end())
		{
			if (it->second->getLoaded())
				++iResLoadedTotal;
			it++;
		}
		return iResLoadedTotal;
	}

	const std::string& GeometryManager::getGroupName(unsigned int iGroupIndex)
	{
		if (iGroupIndex >= group.size())
		{
			std::string err("GeometryManager::getGroupName(");

			err.append(std::to_string(iGroupIndex));
			err.append(") failed. Invalid index given. Number of groups is ");
			err.append(std::to_string(getNumGroups()));
			Log::getPointer()->exception(err);
		}
		std::map<std::string, Group*>::iterator itg = group.begin();
		unsigned int i = 0;
		while (i < iGroupIndex)
		{
			itg++;
			++i;
		}
		return itg->first;
	}

	void GeometryManager::addNewGroup(const std::string& strNewGroupName)
	{
		if (groupExists(strNewGroupName))
		{
			return;
		}
		Group* pNewGroup = new Group;
		group[strNewGroupName] = pNewGroup;
	}

	bool GeometryManager::groupExists(const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator it = group.find(strGroupName);
		if (it == group.end())
			return false;
		return true;
	}

	void GeometryManager::loadGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("GeometryManager::loadGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist");
			Log::getPointer()->exception(err);
		}

		// Load any unloaded resources within the group
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);

		// For each 2d texture resource in this group...
		std::map<std::string, Geometry*>::iterator it = itg->second->resource.begin();
		while (it != itg->second->resource.end())
		{
			if (!it->second->loaded)
			{
				it->second->load();
				it->second->loaded = true;
			}
			it++;
		}
	}

	void GeometryManager::loadGroupSingle(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("GeometryManager::loadGroupSingle(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist");
			Log::getPointer()->exception(err);
		}

		// Load any unloaded resources within the group
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);

		// For each 2d texture resource in this group...
		std::map<std::string, Geometry*>::iterator it = itg->second->resource.begin();
		while (it != itg->second->resource.end())
		{
			if (!it->second->loaded)
			{
				it->second->load();
				it->second->loaded = true;
				return;	// We've changed a resource from unloaded to loaded state, aka, we've loaded a resource, simply return.
			}
			it++;
		}
	}

	void GeometryManager::unloadGroup(const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("GeometryManager::unloadGroup(\"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group doesn't exist");
			Log::getPointer()->exception(err);
		}

		// Unload any loaded resources within the group
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);

		// For each 2d texture resource in this group...
		std::map<std::string, Geometry*>::iterator it = itg->second->resource.begin();
		while (it != itg->second->resource.end())
		{
			if (it->second->loaded)
			{
				it->second->unload();
				it->second->loaded = false;
			}
			it++;
		}
	}

	void GeometryManager::add(const std::string& strNewResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("GeometryManager::add(\"");
			err.append(strNewResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the new resource was to be placed into, doesn't exist!");
			Log::getPointer()->exception(err);
		}

		// Resource already exists in the group?
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);									// Get iterator to the group (we know it exists)
		std::map<std::string, Geometry*>::iterator it = itg->second->resource.find(strNewResourceName);	// Try to find the named resource in the group
		if (itg->second->resource.end() != it)
		{
			it->second->refCount++;
			return;
		}

		// If we get here, we have got to create, then add the resource to the existing named group
		Geometry* pNewRes = new Geometry(strNewResourceName);
		itg->second->resource[strNewResourceName] = pNewRes;
	}

	Geometry* GeometryManager::get(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("GeometryManager::get(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" doesn't exist!");
			Log::getPointer()->exception(err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);				// Get iterator to the group (we know it exists)
		std::map<std::string, Geometry*>::iterator it = itg->second->resource.find(strResourceName);		// Try to find the named resource in the group
		if (itg->second->resource.end() == it)
		{
			std::string err("GeometryManager::get(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. Although the given named group of \"");
			err.append(strGroupName);
			err.append("\" exists, the named resource couldn't be found!");
			Log::getPointer()->exception(err);
		}

		// Is the resource in an unloaded state?
		if (!it->second->loaded)
		{
			// Load it
			it->second->load();
			it->second->loaded = true;
		}
		// Return the resource pointer...
		return (Geometry*)it->second;
	}

	bool GeometryManager::getExists(const std::string& strResourceName, const std::string& strGroupName)
	{
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);
		if (itg == group.end())
			return false;
		std::map<std::string, Geometry*>::iterator it = itg->second->resource.find(strResourceName);
		if (it == itg->second->resource.end())
			return false;
		return true;
	}

	void GeometryManager::remove(const std::string& strResourceName, const std::string& strGroupName)
	{
		// Group doesn't exist?
		if (!groupExists(strGroupName))
		{
			std::string err("GeometryManager::remove(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. As the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, doesn't exist!");
			Log::getPointer()->exception(err);
		}

		// Resource doesn't exist in the group?
		std::map<std::string, Group*>::iterator itg = group.find(strGroupName);									// Get iterator to the group (we know it exists)
		std::map<std::string, Geometry*>::iterator it = itg->second->resource.find(strResourceName);		// Try to find the named resource in the group
		if (itg->second->resource.end() == it)
		{
			std::string err("GeometryManager::remove(\"");
			err.append(strResourceName);
			err.append("\", \"");
			err.append(strGroupName);
			err.append("\") failed. Although the given named group of \"");
			err.append(strGroupName);
			err.append("\" which the resource is supposed to be in, exists, the named resource couldn't be found!");
			Log::getPointer()->exception(err);
		}

		// If we get here, we've found the resource in the group
		// Decrement it's reference count 
		it->second->refCount--;
		// If the reference count is now at zero
		if (it->second->refCount <= 0)
		{
			// If it's in a loaded state, unload it
			if (it->second->loaded)
			{
				it->second->unload();
				it->second->loaded = false;
			}

			// Destroy the resource
			delete it->second;
			itg->second->resource.erase(it);
		}
	}

	void GeometryManager::convertObj(const std::string filename)
	{
		// Used to temporarily hold each line of data from file
		Vector3 vertex;
		Vector3 normal;
		Vector2 texCoord;
		unsigned int index[9];	// Index to vertex, texcoord, normal

		// Used to temporarily hold all unique items from the file
		std::vector<Vector3> vVertices;
		std::vector<Vector3> vNormals;
		std::vector<Vector2> vTexCoords;
		std::vector<unsigned int> vIndicesVertices;
		std::vector<unsigned int> vIndicesNormals;
		std::vector<unsigned int> vIndicesTexCoords;

		FILE* file = 0;
		errno_t err;
		err = fopen_s(&file, filename.c_str(), "r");
		if (err != 0)
		{
			std::string strErr("GeometryManager::convertObj() failed to open file ");
			strErr.append(filename);
			Log::getPointer()->exception(strErr);
		}

		char strLine[255] = { 0 };
		char ch = 0;
		// Go through entire file, loading everything to the above vectors
		while (!feof(file))
		{
			// Get the beginning character of the current line in the file
			ch = fgetc(file);

			if ('v' == ch)	// Could be v(vertex), vn(normal or vt(texcoord)
			{
				ch = fgetc(file);
				if (' ' == ch)	// v(vertex)
				{
					fscanf_s(file, "%f %f %f", &vertex.x, &vertex.y, &vertex.z);
					fgets(strLine, 255, file);	// Read the rest of the line
					vVertices.push_back(vertex);
				}
				else if ('n' == ch)	// n(normal)
				{
					fscanf_s(file, "%f %f %f", &normal.x, &normal.y, &normal.z);
					fgets(strLine, 255, file);	// Read the rest of the line
					vNormals.push_back(normal);
				}
				else if ('t' == ch)	// t(texcoord)
				{
					fscanf_s(file, "%f %f", &texCoord.x, &texCoord.y);
					fgets(strLine, 255, file);	// Read the rest of the line
					vTexCoords.push_back(texCoord);
				}
			}
			else if ('f' == ch)	// f vertex/texcoord/normal indicies
			{
				fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &index[0], &index[1], &index[2], &index[3], &index[4], &index[5], &index[6], &index[7], &index[8]);
				fgets(strLine, 255, file);	// Read the rest of the line
				vIndicesVertices.push_back(index[0]);
				vIndicesTexCoords.push_back(index[1]);
				vIndicesNormals.push_back(index[2]);
				vIndicesVertices.push_back(index[3]);
				vIndicesTexCoords.push_back(index[4]);
				vIndicesNormals.push_back(index[5]);
				vIndicesVertices.push_back(index[6]);
				vIndicesTexCoords.push_back(index[7]);
				vIndicesNormals.push_back(index[8]);
			}
			else if ('\n' == ch)	// Newline
			{
			}
			else // Don't care
			{
				fgets(strLine, 255, file);	// Read the rest of the line
			}
		}
		fclose(file);

		// Now we have everything loaded from the file,
		// save out to binary file.
		std::string strOutputFilename = filename;
		for (int i = 0; i < 3; ++i)
		{
			strOutputFilename.pop_back();
		}
		strOutputFilename += "geom";
		err = fopen_s(&file, strOutputFilename.c_str(), "w+b");
		if (err != 0)
		{
			std::string strErr("GeometryManager::convertObj() failed to open file ");
			strErr.append(filename);
			strErr.append(" for saving.");
			Log::getPointer()->exception(strErr);
		}

		// Loop through each face, generating and saving as we go
		Geometry::Vertex geomVertex;
		unsigned int iNumVertices = (unsigned int)vIndicesVertices.size();
		size_t written = fwrite(&iNumVertices, sizeof(unsigned int), 1, file);
		if (0 == written)
		{
			std::string strErr("GeometryManager::convertObj() failed to write to file ");
			strErr.append(filename);
			Log::getPointer()->exception(strErr);
		}
		for (unsigned int i = 0; i < vIndicesVertices.size(); ++i)
		{
			geomVertex.pos = vVertices[vIndicesVertices[i]-1];		// Vertex position
			geomVertex.normal = vNormals[vIndicesNormals[i]-1];		// Normal
			geomVertex.texCoord = vTexCoords[vIndicesTexCoords[i]-1];	// Texture coordinates
			written = fwrite(&geomVertex, sizeof(Geometry::Vertex), 1, file);
			if (0 == written)
			{
				std::string strErr("GeometryManager::convertObj() failed to write to file ");
				strErr.append(filename);
				Log::getPointer()->exception(strErr);
			}
		}
		fclose(file);
	}
}