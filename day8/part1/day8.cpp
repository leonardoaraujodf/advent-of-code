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
static bool s_first_node = false;
static std::string s_first_node_name = "AAA";

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

    if (s_first_node) {
        s_first_node = false;
        s_first_node_name = n.value;
    }

    s_network_map[n.value] = n;
}

void log_map(void)
{
    for (auto iter = s_network_map.begin(); iter != s_network_map.end(); ++iter)
    {
        const auto node_name = iter->first;
        const auto n = iter->second;
        std::cout << node_name << ": " << n.left << "," << n.right << std::endl;
    }
}

long long iterate_over_map(std::string input)
{
    bool exit = false;
    std::string &current = s_first_node_name;
    long long steps = 0;

    while (!exit) {
        for (auto &c : input) {
            node &n = s_network_map[current];

            if (c == 'L') {
                current = n.left;
                steps++;
            }
            else if (c == 'R') {
                current = n.right;
                steps++;
            }

            if (current.compare("ZZZ") == 0) {
                exit = true;
                break;
            }
            
        }

        std::cout << "I'm out of instruction. Stopped at: " << current << std::endl;
    }

    return steps;
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
    log_map();
    long long steps = iterate_over_map(input);
    std::cout << "Number steps = " << steps << std::endl;
    return 0;
}

