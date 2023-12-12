#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <cctype>

struct map_entry {
    long long destination;
    long long source;
    long long range;

    // Define comparison operator for sorting based on 'source'
    bool operator<(const map_entry& other) const {
        return source < other.source;
    }
};

struct entry {
    long long first;
    long long range;
    bool processed;
};

static const long long MAP_MAX = 7;
static long long current_map = 0;
static std::vector<unsigned int> seeds;
static std::set<map_entry> maps[MAP_MAX];
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
    map_entry map{};

    // check for seeds
    found = line.find("seeds");
    if (found != std::string::npos)
    {
        long long num = 0;
        bool new_num = false;
        for (long unsigned int i = 0; i < line.size(); i++)
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
    for (long long i = 0; i < MAP_MAX; i++)
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

    for (long unsigned int i = 0; i < line.size(); i++)
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
            map_created = true;
        }

        // It's a digit.
        if (state == dest)
        {
            map.destination *= 10;
            map.destination += static_cast<unsigned int>(ch - '0');
        }
        else if (state == source)
        {
            map.source *= 10;
            map.source += static_cast<unsigned int>(ch - '0');
        }
        else if (state == range)
        {
            map.range *= 10;
            map.range += static_cast<unsigned int>(ch - '0');
        }
    }

    if (map_created)
        maps[current_map].insert(map);
}

long long seed_lookup(long long seed_num)
{
    long long input = seed_num;
    long long out = 0;
    bool found = false;
    for (long long i = 0; i < MAP_MAX; i++)
    {
        for (const auto &en : maps[i])
        {
            if (input < en.source)
            {
                // input is less than source. Continue looking for it.
                continue;
            }
            else if (input > (en.source + en.range))
            {
                // input is greater than the source + range. Continue looking for it.
                continue;
            }
            // Found it. It is in the range.
            long long diff = input - en.source;
            out = en.destination + diff;
            found = true;
            break;
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
        // std::cout << "i:" << input << " o:" << out << " ";
        input = out;
    }
    // std::cout << std::endl;

    return out;
}

bool ranges_overlap(const entry &in, const map_entry &me)
{
    if ((in.first + in.range - 1) < me.source)
    {
        return false;
    }
    else if (in.first > (me.source + me.range - 1))
    {
        return false;
    }

    return true;
}

bool range_operation(entry &in, const map_entry &me, std::vector<entry> &out, std::vector<entry> &rest)
{
    if ((in.first >= me.source) && ((in.first + in.range) <= (me.source + me.range)))
    {
        // ranges overlap
        // case 1: input range is inside source range
        long long diff = in.first - me.source;
        out.push_back({me.destination + diff, in.range, false});
        std::cout << "case 1: " << std::endl;
        std::cout << "i: " << in.first << " r: " << in.range << std::endl;
        std::cout << "s: " << me.source << " r: " << me.range << " d:" << me.destination << std::endl;
        std::cout << "o: " << out.back().first << " r: " << out.back().range << std::endl;
        return true;
    }
    else if ((in.first < me.source) && (in.first + in.range <= me.source + me.range))
    {
        // case 2: input range overlaps source range in the right side.
        // In another words, at least the last item of the input range
        // is inside the output range.
        rest.push_back({in.first, me.source - in.first, false}); 
        long long diff = in.first + in.range - me.source;
        out.push_back({me.destination, diff, false});
        std::cout << "case 2: " << std::endl;
        std::cout << "i: " << in.first << " r: " << in.range << std::endl;
        std::cout << "s: " << me.source << " r: " << me.range << " d:" << me.destination << std::endl;
        std::cout << "o: " << out.back().first << " r: " << out.back().range << std::endl;
        std::cout << "re: " << rest.back().first << " r: " << rest.back().range << std::endl;
        return true;
    }
    else if ((in.first >= me.source) && (in.first + in.range > me.source + me.range))
    {
        // case 3: input range overlaps source range in the left side.
        // In another words, at least the first item of the input range
        // is inside the output range.
        rest.push_back({me.source + me.range, (in.first + in.range) - (me.source + me.range), false});
        long long diff = in.first - me.source;
        out.push_back({me.destination + diff, me.source + me.range - in.first, false});
        std::cout << "case 3: " << std::endl;
        std::cout << "i: " << in.first << " r: " << in.range << std::endl;
        std::cout << "s: " << me.source << " r: " << me.range << " d:" << me.destination << std::endl;
        std::cout << "o: " << out.back().first << " r: " << out.back().range << std::endl;
        std::cout << "re: " << rest.back().first << " r: " << rest.back().range << std::endl;
        return true;
    }
    else if ((in.first < me.source) && ((in.first + in.range) > (me.source + me.range)))
    {
        std::cout << "case 4: " << std::endl;
        rest.push_back({in.first, me.source - in.first, false});
        std::cout << "re: " << rest.back().first << " r: " << rest.back().range << std::endl;
        rest.push_back({me.source + me.range, (in.first + in.range) - (me.source + me.range), false});
        std::cout << "re: " << rest.back().first << " r: " << rest.back().range << std::endl;
        out.push_back({me.destination, me.range, false});
        std::cout << "i: " << in.first << " r: " << in.range << std::endl;
        std::cout << "s: " << me.source << " r: " << me.range << " d:" << me.destination << std::endl;
        std::cout << "o: " << out.back().first << " r: " << out.back().range << std::endl;
        return true;
    }

    std::cerr << "entry: f: " << in.first << " r: " << in.range << std::endl;
    std::cerr << "map entry: s: " << me.source << " r: " << me.range << std::endl;
    std::cerr << "Shouldn't be here!" << std::endl;
    exit(EXIT_FAILURE);

    return false;
}

long long seed_range_lookup(long long seed_num, long long range)
{
    std::vector<entry> input;
    std::vector<entry> out;
    std::vector<entry> rest;

    input.push_back({seed_num, range, false});
    for (long long i = 0; i < MAP_MAX; i++)
    {
        for (const auto &en : maps[i])
        {
            for (auto &in : input)
            {
                // Input entry already processed. Skip.
                if (in.processed)
                    continue;

                bool overlap = ranges_overlap(in, en);
                if (!overlap)
                    continue;

                // Input entry is going to be processed now
                in.processed = true;
                range_operation(in, en, out, rest);
            }

            // Get not processed items
            std::vector<entry> input_not_processed;
            for (auto &in : input)
            {
                if (!in.processed)
                    input_not_processed.push_back(in);
            }
            input = input_not_processed;

            if (rest.size())
            {
                for (const auto &r: rest)
                    input.push_back(r);
                rest = {};
            }
        }
        // Any input entry that has not been processed has direct range translation
        for (const auto &in : input)
        {
            if (in.processed)
                continue;
            
            out.push_back(in);
        }

        std::cout << "map: " << MAPS_NAME[i] << std::endl;
        for (const auto &o: out)
            std::cout << "f: " << o.first << " r: " << o.range << std::endl;
        std::cout << std::endl;
        input = out;
        out = {};
    }

    long long low_val = 0;
    long long sum_range = 0;
    bool val_set = false;
    for (const auto &in: input)
    {
        sum_range += in.range;
        if (!val_set)
        {
            low_val = in.first;
            val_set = true;
        }
        else if (in.first < low_val)
            low_val = in.first;
    }

    std::cout << "low_val: " << low_val << std::endl;
    std::cout << "range: " << sum_range << std::endl;
    if (range != sum_range)
        std::cout << "ranges differs!" << std::endl;

    return low_val;
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
    long long lowest_location = 0;
    bool initialized = false;
    while (std::getline(new_file, line))
    {
        load_maps(line);
    }

    std::cout << "seeds & range: ";
    for (long unsigned int i = 0; i < seeds.size() / 2; i++)
    {
        std::cout << "s: " << seeds[2 * i] << " r: " << seeds[2 * i + 1] << " ";
    }
    std::cout << std::endl;
    
    for (long long i = 0; i < MAP_MAX; i++)
    {
        std::cout << "map " << MAPS_NAME[i] << std::endl;
        for (const auto &en : maps[i])
        {
            std::cout << "dst: " << en.destination;
            std::cout << " src: " << en.source;
            std::cout << " ran: " << en.range << std::endl;
        }
    }

    for (long unsigned int i = 0; i < seeds.size() / 2; i++)
    {
        long long location = seed_range_lookup(seeds[2 * i], seeds[2 * i + 1]);
        if (!initialized)
        {
            initialized = true;
            lowest_location = location;
        }
        else if (location < lowest_location)
            lowest_location = location;
    }

    // Brute force strategy
    // for (long long i = 0; i < seeds.size() / 2; i++)
    // {
    //     for (long long j = 0; j < seeds[2 * i + 1]; j++)
    //     {
    //         // long long location = seed_range_lookup(seeds[2 * i], seeds[2 * i + 1]);
    //         long long location = seed_lookup(seeds[2 * i] + j);
    //         if (!initialized)
    //         {
    //             initialized = true;
    //             lowest_location = location;
    //         }
    //         else if (location < lowest_location)
    //             lowest_location = location;
    //     }
    // }

    std::cout << "location = " << lowest_location << std::endl;

    new_file.close();

    return 0;
}

