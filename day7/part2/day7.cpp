#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <set>

static const std::string cards = "AKQT98765432J";
static const std::vector<unsigned int> second_ordering_rule = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

enum hand_type {fivekind, fourkind, fullhouse, threekind, twopair, onepair, highcard};
class Hand {
private:
    std::string m_val;
    hand_type m_ht;
    unsigned int m_bid;
    unsigned int m_score1;
    std::vector<unsigned int> m_score2;
public:
    Hand(std::string v, unsigned int b) : m_val(v), m_bid(b), m_score1(0), m_score2(5,0)
    {
        set_hand_information(m_val);
    }

    void set_hand_information(std::string &hand) {
        unsigned int num_cards_found = 0;
        unsigned int num_jokers = 0;
        std::size_t found = 0;
        std::multiset<unsigned int> occurrences;

        // first verify if there is jokers
        while(num_cards_found < 5)
        {
            found = hand.find('J', found);
            if (found == std::string::npos)
                break;
            num_jokers++;
            num_cards_found++;
            m_score2[found] = 1;
            found++;
        }

        if (num_jokers == 5)
        {
            // If we have 5 jokers then we already have a five of a kind hand
            m_score1 = 3125;
        }
        else
        {
            // Don't search for the joker now
            for (unsigned int i = 0; i < (cards.size() - 1) && num_cards_found < 5; i++)
            {
                char c = cards[i];
                unsigned int occ = 0;
                found = 0;
                
                while (num_cards_found < 5)
                {
                    found = hand.find(c, found);
                    if (found == std::string::npos)
                        break;

                    num_cards_found++;
                    m_score2[found] = second_ordering_rule[i];
                    found++;
                    occ++;
                }
                if (occ != 0)
                    occurrences.insert(occ);
            }

            std::multiset<unsigned int>::iterator last = occurrences.end();
            last--;
            for (std::multiset<unsigned int>::iterator it = occurrences.begin(); it != occurrences.end(); ++it)
            {
                unsigned int partial_score = 1;
                unsigned int total_occ;
                if (num_jokers > 0 && it == last)
                {
                    total_occ = *it + num_jokers;
                }
                else
                {
                    total_occ = *it;
                }

                for (unsigned int i = 0; i < total_occ; i++)
                {
                    partial_score *= 5;
                }
                m_score1 += partial_score;
            }
        }

        const unsigned int POSSIBLE_SCORES[] = {25, 40, 55, 135, 150, 630, 3125};
        for (unsigned int i = 0; i < 7; i++)
        {
            if (m_score1 == POSSIBLE_SCORES[i])
                return;
        }

        std::cerr << "Score not valid!" << std::endl;
        std::cerr << *this;
        exit(EXIT_FAILURE);
    }

    unsigned int get_score1(void) const {
        return m_score1;
    }

    std::string get_hand(void) const {
        return m_val;
    }

    unsigned int get_bid(void) const {
        return m_bid;
    }

    bool operator<(const Hand& other) const {
        if (m_score1 != other.m_score1) {
            return m_score1 < other.m_score1;
        }

        for (unsigned int i = 0; i < m_score2.size(); i++)
        {
            if (m_score2[i] != other.m_score2[i])
                return m_score2[i] < other.m_score2[i];
        }
        std::cout << "Shoudln't be here!" << std::endl;
        exit(EXIT_FAILURE);
        return false;
    }

    friend std::ostream& operator<<(std::ostream &os, const Hand &hand) {
        os << "hand: " << hand.m_val << std::endl;
        os << "bid: " << hand.m_bid << std::endl;
        os << "score1: " << hand.m_score1 << std::endl;
        os << "score2: "; 
        for (unsigned int i = 0; i < hand.m_score2.size(); i++)
        {
            os << hand.m_score2[i] << " ";
        }
        os << std::endl;
        return os;
    }

};

static std::set<Hand> hand_set;

void add_hand(std::string &line)
{
    unsigned int val = 0;
    bool new_val = false;
    std::string hand_str;
    for (const auto &ch : line)
    {
        if (ch == ' ')
        {
            new_val = true;
        }
        else if (!new_val)
        {
            hand_str += ch;
        }
        else if (new_val)
        {
            val *= 10;
            val += ch - '0';
        }
    }

    Hand h(hand_str, val);
    std::cout << h;
    hand_set.insert(h);
}

int main(const int argc, const char * argv[])
{
    std::fstream new_file; 
    std::string line;
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

    while(std::getline(new_file, line))
    {
        add_hand(line);
    }

    unsigned int rank = 1;
    long sum = 0;
    std::cout << "----------------------" << std::endl;
    for (std::set<Hand>::iterator it = hand_set.begin(); it != hand_set.end(); ++it)
    {
        std::cout << "Rank: " << rank << std::endl;
        std::cout << *(it);
        unsigned int val = rank * it->get_bid();
        std::cout << "Val: " << val << std::endl;
        std::cout << "Sum: " << sum << std::endl << std::endl;
        sum += val;
        rank++;
    }

    std::cout << "Sum: " << sum << std::endl;

    return 0;
}
