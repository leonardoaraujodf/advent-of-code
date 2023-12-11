#include <iostream>
#include <fstream>
#include <cctype>

int main()
{
    std::fstream new_file; 
    new_file.open("input.txt", std::ios::in);

    if (!new_file.is_open())
    {
        std::cerr << "Could not open calibration.txt!" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    unsigned int sum = 0;
    while (std::getline(new_file, line))
    {
        int first_val = -1, second_val = -1;
        for (auto &ch : line)
        {
            if(isdigit(ch))
            {
                if (first_val < 0)
                {
                    first_val = ch - '0';
                }
                else
                {
                    second_val = ch - '0';
                }

            }
        }

        if (first_val < 0)
            continue;

        if (second_val < 0)
            second_val = first_val;

        sum += first_val * 10 + second_val;
    }

    std::cout << "Sum is: " << sum << std::endl;


    new_file.close();

    return 0;
}

