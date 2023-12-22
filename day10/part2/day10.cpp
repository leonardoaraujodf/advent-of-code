#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <unordered_map>
#include <cctype>
#include <string>
#include <set>

// @brief Represents a directed graph using adjacency
// list representation.
class Graph {
    // Number of vertices
    int m_v;
    // Map of coordinates "x,y" to vertex num
    std::unordered_map<std::string, unsigned int> m_map;
    // Vector of adjacency lists
    std::vector<std::list<unsigned int>> m_adj;

public:
    // Constructor
    Graph();
    // Function to add a vertex to graph
    unsigned int add_vertex(unsigned int x, unsigned int y);
    // Function to add an edge to graph
    bool add_edge(unsigned int v, unsigned int w);
    bool add_edge(int x1, int y1, int x2, int y2);
    bool get_vertex(unsigned int x, unsigned int y, unsigned int &v_num);
    // Prints BFS traversal from a given source s
    unsigned int get_highest_level(unsigned int s, std::set<unsigned int> &vset);
};

Graph::Graph()
{
    this->m_v = 0;
    m_map.clear();
    m_adj.clear();
}

unsigned int Graph::add_vertex(unsigned int x, unsigned int y)
{
    std::string s; 
    s += std::to_string(x);
    s += ",";
    s += std::to_string(y);
    m_map[s] = m_v;
    m_v++;
    m_adj.push_back({});
    return m_v - 1;
}

bool Graph::add_edge(unsigned int v, unsigned int w)
{
    // add w to v's list
    m_adj[v].push_back(w);
    return true;
}

// Adds vertex at x2,y2 location to vertex at x1,y1
// adjacent list
bool Graph::add_edge(int x1, int y1, int x2, int y2)
{
    unsigned int v, w;
    if (!this->get_vertex(x1, y1, v))
        return false;

    if (!this->get_vertex(x2, y2, w))
        return false;

    this->add_edge(v, w);
    return true;
}

bool Graph::get_vertex(unsigned int x, unsigned int y, unsigned int &v_num)
{
    std::string s;
    s += std::to_string(x);
    s += ",";
    s += std::to_string(y);
    std::unordered_map<std::string, unsigned int>::const_iterator got = m_map.find(s);
    if (m_map.end() == got)
        return false;

    v_num = got->second;
    return true;
}

unsigned int Graph::get_highest_level(unsigned int s, std::set<unsigned int> &vset)
{
    // Mark all vertices as not visited
    std::vector<bool> visited;
    visited.resize(m_v, false);

    // Create a queue for BFS
    std::list<unsigned int> queue;
    unsigned int level = 0;

    // Mark the current node as visited and enqueue it
    visited[s] = true;
    queue.push_back(s);
    vset.insert(s);

    while(!queue.empty()) {
        unsigned int sz = queue.size();
        while (sz--)
        {
            // Dequeue a vertex from queue and print it
            s = queue.front();
            // std::cout << "Vertex " << s << ":" << std::endl;
            queue.pop_front();
            // std::cout << "Adj ";
            for (auto adjacent : m_adj[s]) {
               // std::cout << adjacent << " ";
                if (!visited[adjacent]) {
                    visited[adjacent] = true;
                    queue.push_back(adjacent);
                    vset.insert(adjacent);
                }
            }
            // std::cout << std::endl;
        }
        // std::cout << "Level: " << level << std::endl << std::endl;

        if (!queue.empty())
            level++;
    }

    return level;
}

static Graph s_graph;
static unsigned int s_root_vertex = 0;
static std::set<unsigned int> s_main_loop_vset;

void get_vertexes(std::string &line, std::string &prev, std::string &next, unsigned int y, bool last_line)
{
    unsigned int x = 0;
    for (unsigned int i = 0; i < line.size(); ++i, ++x)
    {
        switch (line[i])
        {
            case '|':
            {
                // No way to have a vertical pipe in the first line
                if (y == 0 || last_line)
                    continue;

                char p = prev[x];

                if ('|' != p && '7' != p && 'F' != p && 'S' != p)
                    continue;

                char n = next[x];
                if ('|' != n && 'L' != n && 'J' != n && 'S' != n)
                    continue;
                
                // Check if vertex exists for previous value
                unsigned int w, v;
                if (!s_graph.get_vertex(x, y - 1, w))
                    continue;

                // Add new vertex
                v = s_graph.add_vertex(x, y);
                s_graph.add_edge(v, w);
                s_graph.add_edge(w, v);
            }
            break;

            case '-':
            {
                // Cannot have horizontal pipe on first and last columns
                if (x == 0 || x == (line.size() - 1))
                    continue;

                char p = line[x - 1];
                if (p != '-' && p != 'L' && p != 'F' && p != 'S')
                    continue;

                char n = line[x + 1];
                if (n != '-' && n != 'J' && n != '7' && n != 'S')
                    continue;

                // Check if vertex exists for previous value
                unsigned int w, v;
                if (!s_graph.get_vertex(x - 1, y, w))
                    continue;
                // Add new vertex
                v = s_graph.add_vertex(x, y);
                s_graph.add_edge(v, w);
                s_graph.add_edge(w, v);
            }
            break;
            
            case 'L':
            {
                // Cannot have an 90-degree N/E bend on first line
                // or last column
                if (y == 0 || x == (line.size() - 1))
                    continue;

                char p = prev[x];
                if (p != '|' && p != '7' && p != 'F' && p != 'S')
                    continue;

                char n = line[x + 1];
                if (n != '-' && n != 'J' && n != '7' && n != 'S')
                    continue;

                // Check if vertex exists for previous value
                unsigned int w, v;
                if (!s_graph.get_vertex(x, y - 1, w))
                    continue;

                // Add new vertex
                v = s_graph.add_vertex(x, y);
                s_graph.add_edge(v, w);
                s_graph.add_edge(w, v);
            }
            break;

            case 'J':
            {
                // Cannot have an 90-degree N/W bend on first line
                // or first column
                if (y == 0 || x == 0)
                    continue;

                char p1 = line[x - 1];
                char p2 = prev[x];
                if (p1 != '-' && p1 != 'L' && p1 != 'F' && p1 != 'S')
                    continue;
                if (p2 != '|' && p2 != '7' && p2 != 'F' && p2 != 'S')
                    continue;

                // Check if vertex exists for previous value
                unsigned int w1, w2, v;
                if (!s_graph.get_vertex(x - 1, y, w1))
                    continue;
                if (!s_graph.get_vertex(x, y - 1, w2))
                    continue;

                // Add new vertex
                v = s_graph.add_vertex(x, y);
                s_graph.add_edge(v, w1);
                s_graph.add_edge(w1, v);
                s_graph.add_edge(v, w2);
                s_graph.add_edge(w2, v);
            }
            break;

            case '7':
            {
                // Cannot have a 90-degree S/W bend on first column
                if (x == 0 || last_line)
                    continue;

                char p = line[x - 1];
                if (p != '-' && p != 'L' && p != 'F' && p != 'S')
                    continue;

                char n = next[x];
                if (n != '|' && n != 'L' && n != 'J' && n != 'S')
                    continue;
                // Check if vertex exists for previous value
                unsigned int w, v;
                if (!s_graph.get_vertex(x - 1, y, w))
                    continue;

                // Add new vertex
                v = s_graph.add_vertex(x, y);
                s_graph.add_edge(v, w);
                s_graph.add_edge(w, v);
            }
            break;

            case 'F':
            {
                if (last_line)
                    continue;

                // Cannot have a 90-degreen S/E bend on last column
                if (x == (line.size() - 1))
                    continue;

                char n = line[x + 1];
                if (n != '-' && n != 'J' && n != '7' && n != 'S')
                    continue;

                n = next[x];
                if (n != '|' && n != 'L' && n != 'J' && n != 'S')
                    continue;

                // We do not care what comes before an F. Just add it.
                s_graph.add_vertex(x, y);
            }
            break;

            case 'S':
            {
                
                // This may come at any position. Just add it.
                s_root_vertex = s_graph.add_vertex(x, y);
                if (x == 0)
                    continue;

                char c = line[x - 1];
                if (c == '-' || c == 'L' || c == 'F')
                {
                    unsigned int w;
                    if (!s_graph.get_vertex(x - 1, y, w))
                        continue;
                    s_graph.add_edge(s_root_vertex, w);
                    s_graph.add_edge(w, s_root_vertex);
                }

                char p = prev[x];
                if (p == '|' || p == '7' || p == 'F')
                {
                    unsigned int w;
                    if (!s_graph.get_vertex(x, y - 1, w))
                        continue;
                    s_graph.add_edge(s_root_vertex, w);
                    s_graph.add_edge(w, s_root_vertex);
                }
            }
            break;

            default:
            break;
        }
    }
}

unsigned int get_inside_tiles(std::string &line, std::string &prev, std::string &next, unsigned int y)
{
    enum states {outside, inside};
    states state = outside;
    unsigned int x = 0;
    unsigned int count = 0;
    // last_bend = 0 means the last bend was not defined yet
    char c, last_bend = 0;
    for (unsigned int i = 0; i < line.size(); ++i, ++x)
    {
        c = line[i];
        unsigned int w;
        bool found = s_graph.get_vertex(x, y, w);
        std::set<unsigned int>::iterator search;
        if (found)
        {
            search = s_main_loop_vset.find(w);
            if (search == s_main_loop_vset.end())
                found = false;
        }

        if (found) {
            switch (c) {
                case '|':
                {
                    if (state == outside)
                        state = inside;
                    else
                        state = outside;
                }
                break;

                case 'L':
                case 'F':
                {
                    if (state == outside)
                        state = inside;
                    else
                        state = outside;
                    last_bend = c;
                }
                break;

                case 'J':
                {
                    if (last_bend == 'L' || last_bend == 'S') {
                        if (state == outside)
                            state = inside;
                        else
                            state = outside;
                    }
                    last_bend = c;
                }
                break;

                case '7':
                {
                    if (last_bend == 'F' || last_bend == 'S') {
                        if (state == outside)
                            state = inside;
                        else
                            state = outside;
                    }
                    last_bend = c;
                }
                break;

                case 'S':
                {
                    bool change_state = false;
                    char p1 = prev[x];
                    char n1 = next[x];
                    char p2 = 0, n2 = 0;
                    if (x > 0)
                        p2 = line[x - 1];
                    if (x < line.size() - 1)
                        n2 = line[x + 1];

                    if (p1 == '|' && n1 == '|') {
                        // It is a |
                        change_state = true;
                    }
                    else if ((n2 == '-' || n2 == '7') &&
                        (n1 == '|' || n1 == 'L' || n1 == 'J')) {
                        last_bend = 'F';
                        change_state = true;
                    }
                    else if ((p2 == '-' || p2 == 'F') &&
                             (n1 == '|' || n1 == 'J' || n1 == 'L')) {
                        if (last_bend == 'F') {
                            change_state = true;
                        }
                        last_bend = '7';
                    }
                    else if ((p2 == '-' || p2 == 'L') &&
                             (p1 == '|' || p1 == '7' || p1 == 'F'))
                    {
                        if (last_bend == 'L') {
                            change_state = true;
                        }
                        last_bend = 'J';
                    }
                    else if ((p1 == '|' || p1 == 'F' || p1 == '7') &&
                             (n2 == '-' || n2 == 'J')) {
                        last_bend = 'L';
                        change_state = true;
                    }

                    if (change_state) {
                        if (state == outside)
                            state = inside;
                        else
                            state = outside;
                    }
                }
                break;

                default:
                break;
            }
        }
        else {
            if (state == inside)
                count++;
        }
    }

    return count;
}

int main(const int argc, const char * argv[])
{
    std::fstream new_file; 
    std::string line;
    std::string current_line;
    std::string previous_line;
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

    unsigned int line_number = 0;
    bool last_line = false;
    while (!last_line)
    {
        std::getline(new_file, line);
        if (line.size() == 0)
            last_line = true;
        if (current_line.size() > 0)
        {
            get_vertexes(current_line, previous_line, line, line_number, last_line);
            line_number++;
        }
        previous_line = current_line;
        current_line = line;
    }

    std::cout << "Highest level: " << s_graph.get_highest_level(s_root_vertex, s_main_loop_vset) << std::endl;

    new_file.close();
    new_file.open(argv[1], std::ios::in);

    line_number = 0;
    unsigned int sum_in_tiles = 0;
    last_line = false;
    while (!last_line)
    {
        std::getline(new_file, line);
        if (line.size() == 0)
            last_line = true;
        if (current_line.size() > 0)
        {
            unsigned int sum = get_inside_tiles(current_line, previous_line, line, line_number);
            std::cout << line_number << ": " << sum << std::endl;
            sum_in_tiles += sum;
            ++line_number;
        }
        previous_line = current_line;
        current_line = line;
    }

    std::cout << "Sum in tiles: " << sum_in_tiles << std::endl;

    return 0;
}
