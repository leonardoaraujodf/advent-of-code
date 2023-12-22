#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

struct coor {
    unsigned int x = 0;
    unsigned int y = 0;
};

static std::vector<std::string> s_matrix;
static std::vector<coor> s_galaxies;

void expand(void)
{
    std::vector<std::string> m1;
    // Expand lines
    for (unsigned int i = 0; i < s_matrix.size(); ++i) {
        auto s = s_matrix[i].find('#');
        if (s == std::string::npos) {
            m1.push_back(s_matrix[i]);
        }
        m1.push_back(s_matrix[i]);
    }

    std::vector<std::string> m2(m1.size(), "");
    for (unsigned int i = 0; i < m1[0].size(); ++i) {
        bool found = false;
        for (unsigned int j = 0; j < m1.size(); ++j)
        {
            char c = m1[j][i];
            if (c != '.') {
                found = true;
            }
            m2[j] += c;
        }

        if (!found) {
            for (auto &s : m2)
                s += '.';
        }
    }

    s_matrix = m2;
    // for (const auto &s : s_matrix)
    //     std::cout << s << std::endl;
}

void get_galaxies(void)
{
    for (unsigned int i = 0; i < s_matrix.size(); ++i) {
        for (unsigned int j = 0; j < s_matrix[i].size(); ++j) {
            if (s_matrix[i][j] == '#') {
                s_galaxies.push_back({i, j});
                std::cout << "Galaxy " << s_galaxies.size() - 1 << " at " << i << "," << j << std::endl;
            }
        }
    }
}

unsigned int get_distance(coor g1, coor g2)
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
    expand();
    get_galaxies();
    auto t = get_distances();
    std::cout << "Total distance = " << t << std::endl;
    return 0;
}

