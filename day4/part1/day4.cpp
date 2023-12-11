#include <iostream>
#include <fstream>
#include <list>
#include <unordered_set>
#include <cctype>

unsigned int get_card_score(std::string &line)
{
    enum states {card_state, winning_num_state, my_number_state};
    char ch;
    std::unordered_set<int> winning_numbers;
    std::list<int> my_numbers;
    enum states state = card_state;
    int num = 0;
    bool new_num = false;
    for (unsigned int i = 0; i < line.size(); i++)
    {
        // char is not a '.' and is not a letter
        ch = line[i];
        if (ch == ':')
        {
            state = winning_num_state;
        }
        else if (winning_num_state == state)
        {
            if (isdigit(ch))
            {
                new_num = true;
                num = num * 10 + (ch - '0');
            }
            else
            {
                if (new_num)
                {
                    winning_numbers.insert(num);
                    num = 0;
                    new_num = false;
                }

                if (ch == '|')
                {
                    state = my_number_state;
                }
            }
        }
        else if (my_number_state == state)
        {
            if (isdigit(ch))
            {
                new_num = true;
                num = num * 10 + (ch - '0');
            }
            else
            {
                if (new_num)
                {
                    my_numbers.push_back(num);
                    num = 0;
                    new_num = false;
                }
            }
        }
    }

    if (new_num)
    {
        my_numbers.push_back(num);
    }

    unsigned int score = 0;
    for (std::list<int>::iterator it = my_numbers.begin(); it != my_numbers.end(); ++it)
    {
        std::unordered_set<int>::const_iterator got = winning_numbers.find(*it);
        if (got != winning_numbers.end())
        {
            std::cout << *it << " ";
            if (0 == score)
                score++;
            else
                score *= 2;
        }
    }

    std::cout << "Line score: " << score << std::endl;
    return score;
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
        sum += get_card_score(line);
    }

    std::cout << "sum: " << sum << std::endl;

    new_file.close();

    return 0;
}

