#include <iostream>
#include <fstream>
#include <cctype>
#include <sstream>
#include <list>
#include <cstdlib>

struct coordinates {
    int x;
    int y;
};

struct symbol_info {
    char symbol;
    struct coordinates c;
};

struct number_info {
    int value;
    int digits;
    bool adj;
    struct coordinates c;
};

struct asterisk_info {
    std::list<number_info *> adj_list;
    struct coordinates c;
};

static std::list<symbol_info *> symbol_list;
static std::list<number_info *> number_list;
static std::list<asterisk_info *> asterisk_list;

static unsigned int sum = 0;

void get_symbols(std::string &line, unsigned int line_number)
{
    char ch;
    for (unsigned int i = 0; i < line.size(); i++)
    {
        // char is not a '.' and is not a letter
        ch = line[i];
        if (ch != '.' && !isdigit(ch))
        {
            symbol_info * s = new symbol_info;
            s->symbol = ch;
            s->c.x = i;
            s->c.y = line_number;
            symbol_list.push_back(s);
        }
    }
}

void get_numbers(std::string &line, unsigned int line_number)
{
    struct number_info * ni = nullptr;
    bool new_number = false;
    char ch;
    for (unsigned int i = 0; i < line.size(); i++)
    {
        ch = line[i];
        if (isdigit(ch))
        {
            if (!new_number)
            {
                // First number digit found
                ni = new number_info;
                ni->value = ch - '0';
                ni->digits = 1;
                ni->adj = false;
                ni->c.x = i;
                ni->c.y = line_number;
                new_number = true;
            }
            else
            {
                // Add more digits to the number
                ni->value = ni->value * 10 + (ch - '0');
                ni->digits++;
            }
        }
        else
        {
            if (new_number)
            {
                // A new number info needs to be added to the list
                number_list.push_back(ni);
                new_number = false;
            }
        }
    }

    if (new_number)
    {
        number_list.push_back(ni);
    }
}

void get_asterisks(std::string &line, int line_number)
{
    asterisk_info * as = nullptr;
    char ch;
    for (unsigned int i = 0; i < line.size(); i++)
    {
        ch = line[i];
        if (ch == '*')
        {
            as = new asterisk_info;
            as->c.x = i;
            as->c.y = line_number;
            asterisk_list.push_back(as);
        }
    }
}

void check_adjcency(void)
{
    for (std::list<symbol_info *>::iterator sit = symbol_list.begin(); sit != symbol_list.end(); ++sit)
    {
        std::cout << "Symbol: " << (*sit)->symbol << " (" << (*sit)->c.x << "," << (*sit)->c.y << ")" << std::endl;
        symbol_info * s = (*sit);
        for (std::list<number_info *>::iterator nit = number_list.begin(); nit != number_list.end(); ++nit)
        {
            number_info * n = (*nit);
            // Check if they are close in the y-axis
            if (abs(s->c.y - n->c.y) > 1)
                continue;

            // Compare each number's digit adjacency
            for (int i = 0; i < n->digits; i++)
            {
                if (abs(s->c.x - (n->c.x + i)) <= 1)
                {
                    n->adj = true;
                    std::cout << "\tNumber: " << (*nit)->value << " D:" << (*nit)->digits << " a:" << (*nit)->adj << " (" << (*nit)->c.x << "," << (*nit)->c.y << ")" << std::endl;
                    break;
                }
            }
        }

    }
}

void check_asterisks(void)
{
    for (std::list<asterisk_info *>::iterator ait = asterisk_list.begin();
         ait != asterisk_list.end(); ++ait)
    {
        asterisk_info * as = (*ait);
        std::cout << "Asterisk: (" << as->c.x << "," << as->c.y << ")" << std::endl;
        for (std::list<number_info *>::iterator nit = number_list.begin();
             nit != number_list.end(); ++nit)
        {
            number_info * n = (*nit);
            
            // Check if they are close in the y-axis
            if (abs(as->c.y - n->c.y) > 1)
                continue;

            // Compare each number's digit adjacency with the asterisk
            for (int i = 0; i < n->digits; i++)
            {
                if (abs(as->c.x - (n->c.x + i)) <= 1)
                {
                    std::cout << "\tNumber: " << (*nit)->value << " D:" << (*nit)->digits << " (" << (*nit)->c.x << "," << (*nit)->c.y << ")" << std::endl;
                    as->adj_list.push_back(n);
                    break;
                }
            }
        }

        // Check now if asterisk is a gear
        if (as->adj_list.size() == 2)
        {
            std::cout << "\tIt's a gear." << std::endl;
            unsigned int gear_ratio = 1;
            for (std::list<number_info *>::iterator inn = as->adj_list.begin();
                 inn != as->adj_list.end();
                 ++inn)
            {
                number_info * m = (*inn);
                gear_ratio *= m->value;
            }
            std::cout << "\tGear ratio: " << gear_ratio << std::endl;
            sum += gear_ratio;
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
    int line_number = 0;
    while (std::getline(new_file, line))
    {
        // get_symbols(line, line_number);
        get_numbers(line, line_number);
        get_asterisks(line, line_number);
        line_number++;
    }

    // check_adjcency();
    check_asterisks();
    //
    // std::cout << "---------------------" << std::endl;
    // for (std::list<symbol_info *>::iterator it = symbol_list.begin(); it != symbol_list.end(); ++it)
    // {
    //      std::cout << "Symbol: " << (*it)->symbol << " (" << (*it)->c.x << "," << (*it)->c.y << ")" << std::endl;
    // }

    // for (std::list<number_info *>::iterator it = number_list.begin(); it != number_list.end(); ++it)
    // {
    //     std::cout << "Number: " << (*it)->value << " D:" << (*it)->digits << " a:" << (*it)->adj << " (" << (*it)->c.x << "," << (*it)->c.y << ")" << std::endl;
    //     if ((*it)->adj)
    //         sum += (*it)->value;
    // }

    std::cout << "sum: " << sum << std::endl;

    new_file.close();

    return 0;
}

