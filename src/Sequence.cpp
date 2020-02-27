#include "Sequence.h"

using namespace std;

bool Sequence::theSameColor(int * matrix, int length) {
    for (int i = 0; i < length; i++) {
        if (matrix[i] == 5)
            return true;
    }
    return false;
}

bool Sequence::fiveInARow(int * matrix) {
    for (int i = 0; i < (13 - 4); i++) {
        if (matrix[i] == 1)
            if (matrix[i + 1] == 1)
                if (matrix[i + 2] == 1)
                    if (matrix[i + 3] == 1)
                        if (matrix[i + 4] == 1)
                            return true;
    }
    return false;
}

Card::Power Sequence::theHighestCard(int * matrix, int start_from) {
    for (int i = start_from; i >= (int) Card::Power::_2; i--) {
        if (matrix[i] == 1)
            return (Card::Power) i;
    }
    CLog::instance() << "Error when checking the highest card in sequence. This message should NEVER be displayed.";
    CLog::instance() << "Card::Power is " + to_string(start_from);
    return Card::Power::_2; // if reaches, something is wrong, this is only for not getting compiler angry
}

int Sequence::repeatedFor(int * matrix, int how_much, int omit) {
    for (int i = 0; i < 13; i++) {
        if (i == omit)
            continue;
        if (matrix[i] == how_much)
            return i;
    }
    return -1;
}

Sequence::Sequence(vector < Card * > seq) {
    int power_matrix[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int color_matrix[4] = { 0, 0, 0, 0 };

    for (int i = 0; i < seq.size(); i++) {
        power_matrix[(int) seq[i]->m_power]++;
        color_matrix[(int) seq[i]->m_color]++;
    }

    // Check for ROYAL_FLUSH
    if (theSameColor(color_matrix) && fiveInARow(power_matrix) && power_matrix[Card::Power::_A] == 1) {
        m_seq = ROYAL_FLUSH;
        m_power = Card::Power::_A;
        m_highcard.clear(); // useless, but makes better feeling when reading
    }
    // Check for STRAIFHT_FLUSH
    else if (theSameColor(color_matrix) && fiveInARow(power_matrix)) {
        m_seq = ROYAL_FLUSH;
        m_power = theHighestCard(power_matrix);
        m_highcard.clear();
    }
    // Check for FOUR_OF_A_KIND
    else if (repeatedFor(power_matrix, 4) != -1) {
        m_seq = FOUR_OF_A_KIND;
        m_power = (Card::Power) repeatedFor(power_matrix, 4);
        m_highcard.push_back(theHighestCard(power_matrix));
    }
    // Check for FULL_HOUSE
    else if (repeatedFor(power_matrix, 3) != -1) {
        if (repeatedFor(power_matrix, 2) != -1) {
            m_seq = FULL_HOUSE;
            m_power = (Card::Power) repeatedFor(power_matrix, 3);
            m_highcard.push_back((Card::Power) repeatedFor(power_matrix, 2));
        } else { // Check for THREE_OF_A_KIND
            m_seq = THREE_OF_A_KIND;
            m_power = (Card::Power) repeatedFor(power_matrix, 3);
            m_highcard.push_back(theHighestCard(power_matrix));
            m_highcard.push_back(theHighestCard(power_matrix, (int) m_highcard[0] - 1));
        }
    }
    // Check for FLUSH
    else if (theSameColor(color_matrix)) {
        m_seq = FLUSH;
        m_power = theHighestCard(power_matrix);
        for (int i = 0; i < 4; i++) {
            if (i == 0) // for the first time
                m_highcard.push_back(theHighestCard(power_matrix, (int) m_power - 1));
            else // for each others
                m_highcard.push_back(theHighestCard(power_matrix, (int) m_highcard[i - 1] - 1));
        }
    }
    // Check for STRAIGHT
    else if (!theSameColor(color_matrix) && fiveInARow(power_matrix)) {
        m_seq = STRAIGHT;
        m_power = theHighestCard(power_matrix);
        m_highcard.clear();
    }
    // Check for TWO_PAIRS
    else if (repeatedFor(power_matrix, 2) != -1) {
        if (repeatedFor(power_matrix, 2, repeatedFor(power_matrix, 2)) != -1) {
            m_seq = TWO_PAIRS;
            m_power = (Card::Power) repeatedFor(power_matrix, 2);
            m_highcard.push_back((Card::Power) repeatedFor(power_matrix, 2, repeatedFor(power_matrix, 2)));
            m_highcard.push_back(theHighestCard(power_matrix));
        }
        // Check for ONE_PAIR
        else {
            m_seq = ONE_PAIR;
            m_power = (Card::Power) repeatedFor(power_matrix, 2);
            for (int i = 0; i < 3; i++) {
                if (i == 0) // for the first time
                    m_highcard.push_back(theHighestCard(power_matrix));
                else // for each others
                    m_highcard.push_back(theHighestCard(power_matrix, (int) m_highcard[i - 1] - 1));
            }
        }
    }
    // Only HIGH_CARD left
    else {
        m_seq = HIGH_CARD;
        m_power = theHighestCard(power_matrix);
        for (int i = 0; i < 4; i++) {
            if (i == 0) // for the first time
                m_highcard.push_back(theHighestCard(power_matrix, (int) m_power - 1));
            else // for each others
                m_highcard.push_back(theHighestCard(power_matrix, (int) m_highcard[i - 1] - 1));
        }
    }
}

bool operator< (const Sequence & me, const Sequence & s) {
    if ((int) me.m_seq < (int) s.m_seq)
        return true;
    else if ((int) me.m_seq > (int) s.m_seq)
        return false;
    else /* if ((int) me.m_seq == (int) s.m_seq) */ {
        if ((int) me.m_power < (int) s.m_power)
            return true;
        else if ((int) me.m_power > (int) s.m_power)
            return false;
        else /* if ((int) me.m_power == (int) s.m_power) */
            for (int i = 0; i < min(me.m_highcard.size(), s.m_highcard.size()); i++)
                if ((int) me.m_highcard[i] < (int) s.m_highcard[i])
                    return true;
    }

    return false;
}

bool operator> (const Sequence & me, const Sequence & s) {
    if ((int) me.m_seq < (int) s.m_seq)
        return false;
    else if ((int) me.m_seq > (int) s.m_seq)
        return true;
    else /* if ((int) me.m_seq == (int) s.m_seq) */ {
        if ((int) me.m_power < (int) s.m_power)
            return false;
        else if ((int) me.m_power > (int) s.m_power)
            return true;
        else /* if ((int) me.m_power == (int) s.m_power) */
            for (int i = 0; i < min(me.m_highcard.size(), s.m_highcard.size()); i++)
                if ((int) me.m_highcard[i] > (int) s.m_highcard[i])
                    return true;
    }

    return false;
}

bool operator== (const Sequence & me, const Sequence & s) {
    if ((int) me.m_seq == (int) s.m_seq && (int) me.m_power == (int) s.m_power)
        return true;
    return false;
}
