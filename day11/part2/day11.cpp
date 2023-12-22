#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

struct coor {
    long long x = 0;
    long long y = 0;
};

static std::vector<std::string> s_matrix;
static std::vector<coor> s_galaxies;
static std::vector<long long> s_x_remap;
static std::vector<long long> s_y_remap;
static const unsigned int EXPANSION_FACTOR = 1000000;

void expand(unsigned int expansion_factor)
{
    long long actual_coor = 0;
    // Expand lines (y-axis)
    for (unsigned int i = 0; i < s_matrix.size(); ++i) {
        s_y_remap.push_back(actual_coor);
        auto s = s_matrix[i].find('#');
        if (s == std::string::npos) {
            actual_coor += expansion_factor;
        }
        else
            ++actual_coor;
    }

    actual_coor = 0;
    for (unsigned int i = 0; i < s_matrix[0].size(); ++i) {
        s_x_remap.push_back(actual_coor);
        bool found = false;
        for (unsigned int j = 0; j < s_matrix.size(); ++j) {
            char c = s_matrix[j][i];
            if (c != '.') {
                found = true;
            }
        }

        if (!found) {
            actual_coor += expansion_factor;
        }
        else
            ++actual_coor;
    }

    std::cout << "\t";
    for (const auto &x : s_x_remap) {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    for (unsigned int i = 0; i < s_y_remap.size(); ++i) {
        std::cout << s_y_remap[i] << ":\t" << s_matrix[i] << std::endl;
    }
}

void get_galaxies(void)
{
    for (unsigned int i = 0; i < s_matrix.size(); ++i) {
        for (unsigned int j = 0; j < s_matrix[i].size(); ++j) {
            if (s_matrix[i][j] == '#') {
                // x,y position
                s_galaxies.push_back({s_x_remap[j], s_y_remap[i]});
                std::cout << "Galaxy " << s_galaxies.size() - 1 << " at " << s_x_remap[j] << "," << s_y_remap[i] << std::endl;
            }
        }
    }
}

long long get_distance(coor g1, coor g2)
{
    return abs(g2.y - g1.y) + abs(g2.x - g1.x);
}

long long get_distances(void)
{
    long long sum = 0;
    for (unsigned int i = 0; i < s_galaxies.size(); i++)
    {
        for(unsigned int j = i + 1; j < s_galaxies.size(); j++)
        {
            auto d = get_distance(s_galaxies[i], s_galaxies[j]);
            std::cout << "G" << i << " to " << "G" << j;
            std::cout << " = " << d << std::endl;
            sum += d;
        }
    }
    return sum;
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
    while (std::getline(new_file, line))
    {
        s_matrix.push_back(line);
    }
    expand(EXPANSION_FACTOR);
    get_galaxies();
    auto t = get_distances();
    std::cout << "Total distance = " << t << std::endl;
    return 0;
}

