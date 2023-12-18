#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <unordered_map>
#include <cctype>
#include <string>

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
    void bfs(int s);
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

void Graph::bfs(int s)
{
    // Mark all vertices as not visited
    std::vector<bool> visited;
    visited.resize(m_v, false);

    // Create a queue for BFS
    std::list<unsigned int> queue;

    // Mark the current node as visited and enqueue it
    visited[s] = true;
    queue.push_back(s);

    while(!queue.empty()) {
        // Dequeue a vertex from queue and print it
        s = queue.front();
        std::cout << s << " ";
        queue.pop_front();

        // Get all adjacent vertices of the dequeued
        // vertex s.
        // If an adjacent has not been visited,
        // then mark it visited and enqueue it
        for (auto adjacent : m_adj[s]) {
            if (!visited[adjacent]) {
                visited[adjacent] = true;
                queue.push_back(adjacent);
            }
        }
    }
}

static Graph s_graph;
static unsigned int s_root_vertex = 0;

void get_vertexes(std::string &line, std::string &prev, unsigned int y)
{
    unsigned int x = 0;
    for (unsigned int i = 0; i < line.size(); ++i, ++x)
    {
        switch (line[i])
        {
            case '|':
            {
                // No way to have a vertical pipe in the first line
                if (y == 0)
                    continue;

                char p = prev[x];
                // If previous pipes were from those types there
                // no way to connect
                if ('-' == p || 'L' == p || 'J' == p || '.' == p)
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
                if (p != 'L' && p != 'F' && p != 'S')
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

                char p = line[x - 1];
                if (p != '-' && p != 'L' && p != 'S')
                    continue;
                //
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

            case '7':
            {
                // Cannot have a 90-degree S/W bend on first column
                if (x == 0)
                    continue;

                char p = line[x - 1];
                if (p != '-' && p != 'L' && p != 'F' && p != 'S')
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
                // Cannot have a 90-degreen S/E bend on last column
                if (x == (line.size() - 1))
                    continue;

                // We do not care what comes before an F. Just add it.
                s_graph.add_vertex(x, y);
            }
            break;

            case 'S':
            {
                
                // This may come at any position. Just add it.
                s_root_vertex = s_graph.add_vertex(x, y);
            }
            break;

            default:
            break;
        }
    }
}

int main(const int argc, const char * argv[])
{
    std::fstream new_file; 
    std::string line;
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
    while (std::getline(new_file, line))
    {
        get_vertexes(line, previous_line, line_number);
        previous_line = line;
        line_number++;
    }
    return 0;
}
