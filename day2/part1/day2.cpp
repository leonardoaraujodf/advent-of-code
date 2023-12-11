#include <iostream>
#include <fstream>
#include <cctype>
#include <sstream>

const int MAX_BLUE = 14;
const int MAX_GREEN = 13;
const int MAX_RED = 12;

void get_game_number(std::string &line, unsigned int &game_id)
{
    std::stringstream game_id_ss;
    for(unsigned int i = 5; i < line.size() - 5; i++)
    {
        char ch = line[i];
        if (ch == ':')
            break;
        else if (isdigit(ch))
        {
            game_id_ss << ch;
        }
    }
    game_id_ss >> game_id;
    std::cout << "ID: " << game_id << std::endl;
}

bool is_game_possible(std::string &line)
{
    std::string sets_str = line.substr(8);
    std::stringstream ss2;
    int num = 0;
    for (unsigned int i = 0; i < sets_str.size(); i++)
    {
        char ch = sets_str[i];
        if (isdigit(ch))
        {
            num = num * 10 + (ch - '0');
        }
        else if (isalpha(ch))
        {
            ss2 << ch;
            const std::string color = ss2.str();
            if (color.compare("blue") == 0)
            {
                if (num > MAX_BLUE)
                    return false;

                ss2.str("");
                num = 0;
            }
            else if (color.compare("red") == 0)
            {
                if (num > MAX_RED)
                    return false;

                ss2.str("");
                num = 0;
            }
            else if (color.compare("green") == 0)
            {
                if (num > MAX_GREEN)
                    return false;

                ss2.str("");
                num = 0;
            }
        }
    }
    return true;
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
    unsigned int sum = 0;
    while (std::getline(new_file, line))
    {
        unsigned int game_id;
        get_game_number(line, game_id);
        if (is_game_possible(line))
        {
            std::cout << "Game " << game_id << " is possible" << std::endl;
            sum += game_id;
        }
        else
        {
            std::cout << "Game " << game_id << " is impossible" << std::endl;
        }
    }

    std::cout << "Sum: " << sum << std::endl;

    new_file.close();

    return 0;
}

