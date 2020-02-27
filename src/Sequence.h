#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <vector>
#include <algorithm>

#include "Card.h"
#include "CLog.h"

class Sequence {
    friend class Round;

    enum SequenceName {
        HIGH_CARD, // 0 // Wysoka karta
        ONE_PAIR, // 1 // Para
        TWO_PAIRS, // 2 // Dwie pary
        THREE_OF_A_KIND, // 3 // Trójka
        STRAIGHT, // 4 // Strit
        FLUSH, // 5 // Kolor
        FULL_HOUSE, // 6 // Full
        FOUR_OF_A_KIND, // 7 // Kareta
        STRAIGHT_FLUSH, // 8 // Poker
        ROYAL_FLUSH // 9 // Poker królewski // not really needed if using STRAIGHT_FLUSH with second parameter
    } m_seq;
    enum Card::Power m_power; // power of sequence
    std::vector < Card::Power > m_highcard; // high card from the rest of hands, from the strongest

    // methods for checking sequences
    bool theSameColor(int * matrix, int length = 4);
    bool fiveInARow(int * matrix);
    Card::Power theHighestCard(int * matrix, int start_from = (int) Card::Power::_A);
    int repeatedFor(int * matrix, int how_much, int omit = -1);

public:
    Sequence(std::vector < Card * > seq); // initialize sequence using cards and calculate power
    friend bool operator< (const Sequence & me, const Sequence & s);
    friend bool operator> (const Sequence & me, const Sequence & s);
    friend bool operator== (const Sequence & me, const Sequence & s);
};

#endif
