#include <fstream>

using namespace std;

class Object
{
	private:
		ifstream *_obj_file;

	public:
		Object(std::string filename);
		~Object();
};
