#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>

struct map_entry {
    unsigned int destination;
    unsigned int source;
    unsigned int range;
};

static const unsigned int MAP_MAX = 7;
static unsigned int current_map = 0;
static std::vector<unsigned int> seeds;
static std::vector<map_entry*> maps[MAP_MAX];
static const std::string MAPS_NAME[MAP_MAX] =
{
    "seed-to-soil",
    "soil-to-fertilizer",
    "fertilizer-to-water",
    "water-to-light",
    "light-to-temperature",
    "temperature-to-humidity",
    "humidity-to-location"
};

void load_maps(std::string &line)
{
    enum states {dest, source, range};

    std::size_t found;
    states state = dest;
    bool map_created = false;
    map_entry *map = nullptr;

    // check for seeds
    found = line.find("seeds");
    if (found != std::string::npos)
    {
        unsigned int num = 0;
        bool new_num = false;
        for (unsigned int i = 0; i < line.size(); i++)
        {
            char ch = line[i];
            if (!isdigit(ch))
            {
                if (new_num)
                {
                    seeds.push_back(num);
                    new_num = false;
                    num = 0;
                }
                continue;
            }

            new_num = true;
            num *= 10;
            num += static_cast<unsigned int>(ch - '0');
        }
        // Store last num
        if (new_num)
        {
            seeds.push_back(num);
        }
        return;
    }
    // check for map changing
    for (unsigned int i = 0; i < MAP_MAX; i++)
    {
        found = line.find(MAPS_NAME[i]);
        if (found == std::string::npos)
            continue;
        else
        {
            current_map = i;
            return;
        }
    }

    for (unsigned int i = 0; i < line.size(); i++)
    {
        char ch = line[i];
        if (!isdigit(ch))
        {
            // It's a space. Change state.
            if (state == dest)
                state = source;
            else if (state == source)
                state = range;
            continue;
        }

        if (!map_created)
        {
            map = new map_entry{};
            map_created = true;
        }

        // It's a digit.
        if (state == dest)
        {
            map->destination *= 10;
            map->destination += static_cast<unsigned int>(ch - '0');
        }
        else if (state == source)
        {
            map->source *= 10;
            map->source += static_cast<unsigned int>(ch - '0');
        }
        else if (state == range)
        {
            map->range *= 10;
            map->range += static_cast<unsigned int>(ch - '0');
        }
    }

    if (map_created)
        maps[current_map].push_back(map);
}

unsigned int seed_lookup(unsigned int seed_num)
{
    unsigned int input = seed_num;
    unsigned int out = 0;
    bool found = false;
    for (unsigned int i = 0; i < MAP_MAX; i++)
    {
        for (unsigned int j = 0; j < maps[i].size(); j++)
        {
            if (input < maps[i][j]->source)
            {
                // input is less than source. Continue looking for it.
                continue;
            }
            else if (input > (maps[i][j]->source + maps[i][j]->range))
            {
                // input is greater than the source + range. Continue looking for it.
                continue;
            }
            // Found it. It is in the range.
            unsigned int diff = input - maps[i][j]->source;
            out = maps[i][j]->destination + diff;
            found = true;
        }

        if (!found)
        {
            // Value was not within a range. 
            // Source corresponds to destination.
            out = input;
        }
        else
        {
            // Value was found within a range.
            found = false;
        }
        // Update new input
        std::cout << "i:" << input << " o:" << out << " ";
        input = out;
    }
    std::cout << std::endl;

    return out;
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
    unsigned int lowest_location = 0;
    bool initialized = false;
    while (std::getline(new_file, line))
    {
        load_maps(line);
    }

    std::cout << "seeds ";
    for (unsigned int i = 0; i < seeds.size(); i++)
    {
        std::cout << seeds[i] << " ";
    }
    std::cout << std::endl;
    
    for (unsigned int i = 0; i < MAP_MAX; i++)
    {
        std::cout << "map " << MAPS_NAME[i] << std::endl;
        for (unsigned int j = 0; j < maps[i].size(); j++)
        {
            std::cout << "dst: " << maps[i][j]->destination;
            std::cout << " src: " << maps[i][j]->source;
            std::cout << " ran: " << maps[i][j]->range << std::endl;
        }
    }

    for (unsigned int i = 0; i < seeds.size(); i++)
    {
        unsigned int location = seed_lookup(seeds[i]);
        if (!initialized)
        {
            initialized = true;
            lowest_location = location;
        }
        else if (location < lowest_location)
            lowest_location = location;
    }

    std::cout << "location = " << lowest_location << std::endl;

    new_file.close();

    return 0;
}

