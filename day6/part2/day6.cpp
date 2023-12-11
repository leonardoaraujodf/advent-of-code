#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>

std::vector<long long> times;
std::vector<long long> distances;

void get_values(std::string &line, std::vector<long long> &vals)
{
    long long val = 0;
    bool new_val = false;
    for (const auto &ch : line)
    {
        if (isdigit(ch))
        {
           new_val = true;
           val = val * 10 + ch - '0';
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

    std::vector<long long> ways;
    for (long long i = 0; i < times.size(); i++)
    {
        ways.push_back(0);
        for (long long h = 1; h < times[i]; h++)
        {
            long long speed = h;
            long long distance = speed * (times[i] - h);
            // std::cout << "h:" << h << " d:" << distance << " ";
            if (distance > distances[i])
                ways[i] += 1;
        }
        std::cout << std::endl;
        std::cout << "ways: " << ways[i] << std::endl;
    }

    long long fin = 1;
    for (const auto w : ways)
        fin *= w;

    std::cout << "Final value: " << fin << std::endl;

    return 0;
}

