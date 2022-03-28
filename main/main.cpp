#include "../read_boolean_write.h"

int main(int argc, char *argv[])
{
  read_boolean_write(argv[1],std::string(argv[1])+".main.xml");
}
