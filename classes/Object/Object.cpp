#include "Object.hpp"

Object::Object(std::string file_name)
{
	_obj_file = new ifstream(file_name);
}

Object::~Object()
{
	delete _obj_file;
}
