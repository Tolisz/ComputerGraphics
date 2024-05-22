#include <iostream>

#include <glm/vec4.hpp>

int main() 
{
    glm::vec4 v(1.0f);

    std::cout << v.x << " " << v.y << " " << v.z << " " << v.w << " " << std::endl;
    return 0;
}