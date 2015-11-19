#include "utility.h"

std::ostringstream file_name(path_t &path)
{
	std::ostringstream fileName;
	fileName << path.dir << "\\" << path.name << "." << path.ext;
	return fileName;	
}

std::ostringstream file_name(path_t &path, int it)
{
	std::ostringstream fileName;
	fileName << path.dir << "\\" << path.name << it << "." << path.ext;
	return fileName;	
}