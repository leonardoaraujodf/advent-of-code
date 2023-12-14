#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <unordered_map>

struct node {
    std::string value;
    std::string left;
    std::string right;
};

static std::unordered_map<std::string, node> s_network_map;

static void get_node(std::string &line)
{
    enum states {value, left, right};
    states s = value;
    node n;
    for (const auto c : line)
    {
        if (isalpha(c)) {
            if (s == value)
                n.value += c;
            else if (s == left)
                n.left += c;
            else if (s == right)
                n.right += c;
        }
        else {
            if (s == value && c == '(')
                s = left;
            else if (s == left && c == ' ')
                s = right;
        }
    }
    s_network_map[n.value] = n;
}

int main(const int argc, const char * argv[])
{
    std::fstream new_file; 
    std::string line;
    std::string input;
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

    // Get the LR.. input
    std::getline(new_file, input);

    // Get rid of the second line
    std::getline(new_file, line);
    while (std::getline(new_file, line))
    {
        get_node(line);
    }
    return 0;
}

