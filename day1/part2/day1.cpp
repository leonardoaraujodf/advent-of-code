#include <iostream>
#include <fstream>
#include <cctype>

static const std::string s_digits[9] = {"one", "two", "three", "four",
                                        "five", "six", "seven", "eight", "nine"};

void check_for_values(std::string &line, int &val1, int &val2,
                      int &pos1, int &pos2)
{
    val1 = val2 = pos1 = pos2 = -1;
    unsigned int pos = 0;
    for (auto &ch: line)
    {
        if(isdigit(ch))
        {
            if (val1 < 0)
            {
                val1 = ch - '0';
                pos1 = pos;
            }
            
            if ((val2 < 0) || ((int)pos > pos2))
            {
                val2 = ch - '0';
                pos2 = pos;
            }
        }
        pos++;
    }

    // Nothing to do here because this case is not expected
    if (val1 < 0)
        return;

    if (val2 < 0)
    {
        val2 = val1;
        pos2 = pos1;
    }
}

void check_for_string_digits(std::string &line, int &val1, int &val2,
                             int &pos1, int &pos2)
{
    val1 = val2 = pos1 = pos2 = -1;
    for(unsigned int i = 0; i < 9; ++i)
    {
        std::size_t found = line.find(s_digits[i]);
        if (std::string::npos == found)
            continue;

        if ((pos1 < 0) || ((int)found < pos1))
        {
            pos1 = found;
            val1 = i + 1;
        }

        while (std::string::npos != found)
        {
            if ((int)found > pos2)
            {
                pos2 = found;
                val2 = i + 1;
            }
            found = line.find(s_digits[i], found + 1);
            std::cout << "iter" << i + 1;
        }
        std::cout << std::endl;
    }

    // Nothing to do here because this case is not expected
    if (pos1 < 0)
        return;

    if (pos2 < 0)
    {
        pos2 = pos1;
        val2 = val1;
    }
}

int main()
{
    std::fstream new_file; 
    new_file.open("input.txt", std::ios::in);
    // new_file.open("calibration.txt", std::ios::in);

    if (!new_file.is_open())
    {
        std::cerr << "Could not open calibration.txt!" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    unsigned int sum = 0;
    while (std::getline(new_file, line))
    {
        int val1, val2, pos1, pos2;
        int val1_str, val2_str, val1_str_pos, val2_str_pos;

        check_for_values(line, val1, val2, pos1, pos2);
        check_for_string_digits(line, val1_str, val2_str, val1_str_pos, val2_str_pos);

        if (pos1 < 0 && val1_str_pos < 0)
        {
            std::cout << "HERE!!!" << std::endl;
        }

        if ((pos1 < 0) || (!(val1_str_pos < 0) && (val1_str_pos < pos1)))
        {
            val1 = val1_str;
        }
        
        if ((pos2 < 0) || (val2_str_pos > pos2))
        {
            val2 = val2_str;
        }

        std::cout << line << " -> " << val1 * 10 + val2 << std::endl;
        sum += val1 * 10 + val2;
    }

    std::cout << "Sum is: " << sum << std::endl;


    new_file.close();

    return 0;
}
