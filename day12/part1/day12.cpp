#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cctype>

class Record {
public:
    std::string m_line;
    std::vector<unsigned int> m_ranges;
    std::vector<unsigned int> m_qlocations;
    Record(std::string line, std::vector<unsigned int> &ranges, std::vector<unsigned int> qlocations) : m_line(line), m_ranges(ranges), m_qlocations(qlocations) {}
};

static std::vector<Record> s_records;

bool is_an_arrangment(const std::string &row, const std::vector<unsigned int> &ran)
{
    
    unsigned int total_damaged = 0;
    unsigned int total = 0;
    std::size_t found = 0;
    for (const auto &c : row)
    {
        if (c == '#')
            ++total_damaged;
    }

    for (const auto &r : ran)
        total += r;

    if (total_damaged != total)
        return false;

    for (const auto &r : ran)
    {
       std::string damaged = "";
       for (unsigned int i = 0; i < r; ++i)
           damaged += "#";

       found = row.find(damaged, found);
       if (found == std::string::npos) {
           return false;
       } else if (found == 0) {
           if (row[found + damaged.size()] != '.')
               return false;
       } else {
           if (row[found - 1] != '.')
               return false;
           else if ((found + damaged.size() < row.size()) && row[found + damaged.size()] != '.')
               return false;
       }
       found += damaged.size();
    }

   return true;
}

long long get_num_arrangments(const Record &rec)
{
    long long comb = 1;
    long long count = 0;

    for (unsigned int i = 0; i < rec.m_qlocations.size(); ++i)
        comb *= 2;

    for (long long i = 0; i < comb; ++i)
    {
        std::string temp = rec.m_line;
        long long val = i;
        for (const auto q : rec.m_qlocations)
        {
            if (val & 0x01)
                temp[q] = '#';
            else
                temp[q] = '.';
            val >>= 1;
        }

        if (is_an_arrangment(temp, rec.m_ranges))
            ++count;
    }

    return count;
}

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

    Record r(srow, ranges, q_locations);
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

    long long count = 0;
    for (const auto &r : s_records)
    {
        std::cout << r.m_line << std::endl;
        long long c = get_num_arrangments(r);
        std::cout << "t = " << c << std::endl;
        count += c;
    }
    std::cout << "Total = " << count << std::endl;
    return 0;
}

