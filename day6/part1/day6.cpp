#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>

std::vector<unsigned int> times;
std::vector<unsigned int> distances;

void get_values(std::string &line, std::vector<unsigned int> &vals)
{
    unsigned int val = 0;
    bool new_val = false;
    for (const auto &ch : line)
    {
        if (isdigit(ch))
        {
           new_val = true;
           val = val * 10 + ch - '0';
        }
        else if (new_val)
        {
            new_val = false;
            vals.push_back(val);
            val = 0;
        }
    }

    if (new_val)
        vals.push_back(val);

    for (const auto &v: vals)
    {
        std::cout << v << " ";
    }
    std::cout << std::endl;
}

int main(const int argc, const char * argv[])
{
    std::fstream new_file; 
    if (argc != 2)
    {
        std::cerr << "Invalid arguments!" << std::endl;
        exit(EXIT_FAILURE);
    }

    new_file.open(argv[1], std::ios::in);

    if (!new_file.is_open())
    {
        std::cerr << "Could not open " << argv[1] << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    std::cout << "Time: ";
    std::getline(new_file, line);
    get_values(line, times);
    std::cout << "Distance: ";
    std::getline(new_file, line);
    get_values(line, distances);

    std::vector<unsigned int> ways;
    for (unsigned int i = 0; i < times.size(); i++)
    {
        ways.push_back(0);
        for (unsigned int h = 1; h < times[i]; h++)
        {
            unsigned int speed = h;
            unsigned int distance = speed * (times[i] - h);
            std::cout << "h:" << h << " d:" << distance << " ";
            if (distance > distances[i])
                ways[i] += 1;
        }
        std::cout << std::endl;
        std::cout << "ways: " << ways[i] << std::endl;
    }

    unsigned int fin = 1;
    for (const auto w : ways)
        fin *= w;

    std::cout << "Final value: " << fin << std::endl;

    return 0;
}

