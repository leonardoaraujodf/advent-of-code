#include <iostream>
#include <fstream>
#include <list>
#include <unordered_set>
#include <vector>
#include <cctype>

static std::vector<int> cardscore;

void get_card_score(std::string &line, unsigned int line_number)
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
            score++;
        }
    }
    std::cout << std::endl;

    if (cardscore.size() < (line_number + 1))
    {
        // Score for the card does not exist yet
        cardscore.emplace_back(1);
    }
    else
    {
        // Compute score for the original card
        cardscore[line_number] = cardscore[line_number] + 1;
    }

    if (score > 0)
    {
        for (unsigned int i = 0; i < (unsigned int)cardscore[line_number]; i++)
        {
            for (unsigned int j = 0; j < score; j++)
            {
                if ((cardscore.size() - 1) < (line_number + j + 1))
                {
                    cardscore.emplace_back(1);
                }
                else
                {
                    cardscore[line_number + j + 1] = cardscore[line_number + j + 1] + 1;
                }
            }
        }

        for (unsigned int i = 0; i < cardscore.size(); i++)
        {
            std::cout << "\tCard " << i + 1 << ": " << cardscore[i] << std::endl;
        }
    }
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
    unsigned int line_number = 0;
    while (std::getline(new_file, line))
    {
        get_card_score(line, line_number);
        line_number++;
    }
    
    std::cout << "----------------------------------" << std::endl;
    for (unsigned int i = 0; i < cardscore.size(); i++)
    {
        std::cout << "Card " << i + 1 << ": " << cardscore[i] << std::endl;
        sum += cardscore[i];
    }

    std::cout << "Sum: " << sum << std::endl;
    new_file.close();

    return 0;
}

