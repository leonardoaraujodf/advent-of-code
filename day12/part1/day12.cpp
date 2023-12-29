#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cctype>

class Record {
private:
    std::string m_line;
    std::vector<unsigned int> m_ranges;
    std::vector<unsigned int> m_qlocations;
public:
    Record(std::string line, std::vector<unsigned int> ranges) : m_line(line), m_ranges(ranges) {}
};

static std::vector<Record> s_records;

void get_record(std::string &line)
{
    std::string srow;
    std::vector<unsigned int> ranges;
    std::vector<unsigned int> q_locations;
    unsigned int loc = 0;
    enum states {row, range};
    states state = row;
    unsigned int num = 0;
    for (const auto &c : line) {
        if (state == row) {
            if (c == '.' || c == '#' || c == '?') {
                srow += c;
                if (c == '?') {
                    q_locations.push_back(loc);
                }
            } else if (c == ' ') {
                state = range;
            }
            ++loc;
        } else if (state == range) {
            if (isdigit(c)) {
                num *= 10;
                num += c - '0';
            } else if (c == ',') {
                ranges.push_back(num);
                num = 0;
            }
        }
    }
    ranges.push_back(num);

    std::cout << "Row: " << srow << std::endl;
    for (const auto &x : ranges)
        std::cout << x << " ";
    std::cout << std::endl;

    Record r(srow, ranges);
    s_records.push_back(r);
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
        get_record(line);
    }
    return 0;
}

