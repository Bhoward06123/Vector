#include "include/Vector.h"

int main()
{
    Vector<int> v1(5);

    std::cout << "size: " << v1.size() << std::endl;
    std::cout << "capacity: " << v1.capacity() << std::endl;

    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    v1.push_back(4);
    v1.push_back(5);

    std::cout << "elements: ";
    for(int i = 0; i < v1.size(); i++)
    {
        std::cout << v1[i] << " ";
    }
    std::cout << std::endl;

    v1.insert(2, 10);

    std::cout << "elements after insert: ";
    for(int i = 0; i < v1.size(); i++)
    {
        std::cout << v1[i] << " ";
    }
    std::cout << std::endl;

    v1.erase(4);

    std::cout << "elements after erase: ";
    for(int i = 0; i < v1.size(); i++)
    {
        std::cout << v1[i] << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
