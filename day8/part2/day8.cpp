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
static std::vector<node> s_start_nodes;

static void get_node(std::string &line)
{
    enum states {value, left, right};
    states s = value;
    node n;

    for (const auto c : line)
    {
        if (isalpha(c) || isdigit(c)) {
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

    if (n.value[2] == 'A')
        s_start_nodes.push_back(n);

    s_network_map[n.value] = n;
}

void log_map(void)
{
    std::cout << "Starting values:" << std::endl;
    for (const auto &n : s_start_nodes)
    {
        std::cout << n.value << ": " << n.left << "," << n.right << std::endl;
    }
    std::cout << std::endl;
    for (auto iter = s_network_map.begin(); iter != s_network_map.end(); ++iter)
    {
        const auto node_name = iter->first;
        const auto n = iter->second;
        std::cout << node_name << ": " << n.left << "," << n.right << std::endl;
    }
}

long long iterate_over_map(std::string input, std::string node_name)
{
    bool exit = false;
    std::string &current = node_name;
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

            if (current[2] == 'Z') {
                exit = true;
                break;
            }
            
        }

        // std::cout << "I'm out of instruction. Stopped at: " << current << std::endl;
    }

    return steps;
}

long long gcd(long long a, long long b)
{
    while (a > 0 && b > 0) {
        if (a > b) {
            a = a % b;
        }
        else {
            b = b % a;
        }
    }
    if (a == 0) {
        return b;
    }
    return a;
}

long long lcm(long long a, long long b)
{
    return (a / gcd(a,b)) * b;
}

// long long iterate_over_map(std::string input)
// {
//     bool exit = false;
//     std::string *current[s_start_nodes.size()];
//     unsigned int found = 0;
//     long long steps = 0;
// 
//     for (unsigned int i = 0; i < s_start_nodes.size(); i++)
//     {
//         current[i] = &s_start_nodes[i].value;
//     }
// 
//     while (!exit) {
//         for (auto &c : input) {
//             node *n[s_start_nodes.size()];
//             steps++;
//             found = 0;
//             for (unsigned int i = 0; i < s_start_nodes.size(); i++)
//             {
//                 n[i] = &s_network_map[*current[i]];
//                 if (c == 'L') {
//                     current[i] = &(n[i]->left);
//                 }
//                 else if (c == 'R') {
//                     current[i] = &(n[i]->right);
//                 }
// 
//                 if ((*current[i])[2] == 'Z') {
//                     found++;
//                     if (found == s_start_nodes.size()) {
//                         exit = true;
//                         break;
//                     }
//                 }
//                 else
//                 {
//                     found = 0;
//                 }
//             }
//         }
//         // std::cout << "I'm out of instruction. Stopped at: " << current << std::endl;
//     }
// 
//     return steps;
// }

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
    std::vector<long long> steps_vec;
    // Get every node number of steps
    for (unsigned int i = 0; i < s_start_nodes.size(); i++)
    {
        long long steps = iterate_over_map(input, s_start_nodes[i].value);
        std::cout << "Node: " << s_start_nodes[i].value << " steps: " << steps << std::endl;
        steps_vec.push_back(steps);
    }
    
    // The final output is just the lhe least common multiple of all outputs found
    long long res = steps_vec[0];
    for (unsigned int i = 0; i < steps_vec.size() - 1; i++)
    {
        res = lcm(res, steps_vec[i + 1]);
    }
    std::cout << "Number steps = " << res << std::endl;
    return 0;
}

