#include <array>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <map>
#include <functional>
struct TestCase { std::string name{}; std::ostringstream failures{}; };
template <typename T> void operator | (TestCase& testCase, T&& testBody) {
    testBody(); auto failures = testCase.failures.str();
    if (failures.empty()) std::cout << testCase.name << ": SUCCESS\n";
    else std::cerr << testCase.name << ": FAILURE\n" << failures;
}  
void addFailure(std::ostream& os, const char* file, unsigned line, const char* condition) {
    os << file << ":" << line << ": Failed: " << condition << "\n";
}
#define TEST(name) TestCase name{#name}; name | [&, &failures = name.failures]
#define EXPECT(cond) if (cond) {} else addFailure(failures, __FILE__, __LINE__, #cond)
#define ASSERT(cond) if (cond) {} else return addFailure(failures, __FILE__, __LINE__, #cond)
enum Suit
{
    Hearts,
    Diamonds,
    Clubs,
    Spades
};
enum Rank
{
    Ace,
    King,
    Queen,
    Jack,
    Ten,
    Nine,
    Eight,
    Seven,
    Six,
    Five,
    Four,
    Three,
    Two
};
struct Card
{
    Rank rank;
    Suit suit;
};

bool isStraight(const std::vector<Card>&);
bool isFlush(const std::vector<Card>& );
std::array<int,13> makeCounts(std::vector<Rank> const &);


std::vector<Rank> toRanks(const std::vector<Card>& cardVec)
{
    std::vector<Rank> handRanks;
    for(const auto &card: cardVec)
    {
        handRanks.push_back(card.rank);
    }
    return handRanks;
}
 
bool isRoyalFlush(const std::vector<Card>& hand)
{
   if( !isFlush(hand))return false;
    std::vector<Rank> RoyalFlush = {Ace, King, Queen, Jack, Ten};
    std::vector<Rank> handRanks;
    for(const auto &card: hand)
    {
        handRanks.push_back(card.rank);
    }
    return std::is_permutation(RoyalFlush.begin(), RoyalFlush.end(), handRanks.begin());
}

bool isStraightFlush(const std::vector<Card>& hand)
{
    return isStraight(hand) && isFlush(hand);
}

bool isFourOfAKind(const std::vector<Card>&hand)
{   auto counts = makeCounts(toRanks(hand));
    return std::find(counts.begin(), counts.end(), 4) != counts.end();
}

bool isFullHouse(const std::vector<Card>&hand)
{   auto counts = makeCounts(toRanks(hand));
    return (std::find(counts.begin(), counts.end(), 3) != counts.end()) && (std::find(counts.begin(), counts.end(), 2) != counts.end());
}

bool isFlush(const std::vector<Card>& hand){
Suit firstCard = hand.at(0).suit;
    auto iterator = std::find_if_not(hand.begin(), hand.end(), [&](Card card){return card.suit == firstCard;});
    return iterator == std::end(hand);
}

bool isStraight(const std::vector<Card>&hand)
{
      std::vector<Rank> handRanks;
    for(const auto &card: hand)
    {
        handRanks.push_back(card.rank);
    }
    std::sort(handRanks.begin(), handRanks.end());
    for(int i = 0; i < 4; i++)
    {
        if(handRanks.at(i) - handRanks.at(i+1) != -1)
        {
            return false;
        }
    }
    return true;  
}

bool isThreeOfAKind(const std::vector<Card>&hand)
{   auto counts = makeCounts(toRanks(hand));
    return std::find(counts.begin(), counts.end(), 3) != counts.end();
}

bool isTwoPairs(const std::vector<Card>&hand)
{   auto counts = makeCounts(toRanks(hand));
    auto firstPair=std::find(counts.begin(), counts.end(), 2);
    if(firstPair==counts.end()) return false;
    *firstPair=0;
    return std::find(counts.begin(), counts.end(), 2) != counts.end();
}

bool isPair(const std::vector<Card>&hand)
{   auto counts = makeCounts(toRanks(hand));
    return std::find(counts.begin(), counts.end(), 2) != counts.end();
}

std::array<int,13> makeCounts(std::vector<Rank> const &ranks)
{
    std::array<int,13> ranksCount{};
    for(auto const rank: ranks)
    {
        ranksCount.at(rank)++;
    }
    return ranksCount;
}

unsigned int checkHand(const std::vector<Card>& hand)
{
std::vector<std::function <bool(std::vector<Card>)>> FunctionContainer{isRoyalFlush,isStraightFlush,isFourOfAKind,isFullHouse,isFlush,isStraight,
                                                                isThreeOfAKind,isTwoPairs,isPair};
    unsigned int FunctionNumber=1;
for(auto func:FunctionContainer)
{
    if(func(hand)) break;
    FunctionNumber++;
}
return FunctionNumber;
}

std::string compareHands(const std::vector<Card>& hand1, const std::vector<Card>& hand2)
{
    const std::vector<std::string> rankStrings {"error","Royal Flush", "Straight Flush", "Four of a kind",
                                                "Full house", "Flush", "Straight", "Three of a kind", "Two pairs", "Pair", "High Card"};
    unsigned int hand1Value = checkHand(hand1);
    unsigned int hand2Value = checkHand(hand2);
    std::stringstream message;
    if(hand1Value == hand2Value)
        return "Draw!";

    if (hand1Value< hand2Value) 
    {
        message << "Player 1 wins with "<<rankStrings.at(hand1Value);
    }
    else
    {
        message << "Player 2 wins with "<<rankStrings.at(hand2Value);
    }
    return message.str();
}

int main() 
 {   
  TEST(compareHandsFunctionWorksCorrectly)
    {
        std::vector<Card> handRoyalFlush{{Ace, Hearts}, {King, Hearts}, {Queen, Hearts}, {Jack, Hearts}, {Ten, Hearts}};
        std::vector<Card> handStraightFlush{{Nine, Hearts}, {King, Hearts}, {Queen, Hearts}, {Jack, Hearts}, {Ten, Hearts}};
        std::vector<Card> handFourOfAKind{{Ace, Hearts}, {Ace, Hearts}, {Ace, Hearts}, {Ace, Hearts}, {Ten, Hearts}};
        std::vector<Card> handFullHouse{{Ace, Hearts}, {King, Hearts}, {Ace, Hearts}, {King, Hearts}, {King, Hearts}};
        std::vector<Card> handFlush{{Two, Hearts}, {King, Hearts}, {Ace, Hearts}, {King, Hearts}, {King, Hearts}};
        std::vector<Card> handStraight{{Nine, Spades}, {King, Clubs}, {Queen, Clubs}, {Jack, Clubs}, {Ten, Clubs}};
        std::vector<Card> handThreeOfAKind{{Ace, Hearts}, {King, Clubs}, {Three, Clubs}, {Three, Hearts}, {Three, Hearts}};
        std::vector<Card> handTwoPairs{{Ace, Hearts}, {King, Clubs}, {Ace, Clubs}, {King, Hearts}, {Nine, Hearts}};
        std::vector<Card> handOnePair{{Ace, Hearts}, {King, Clubs}, {Ace, Clubs}, {Ten, Hearts}, {Nine, Hearts}};
        std::vector<Card> handHighCard{{Ace, Hearts}, {King, Clubs}, {Three, Clubs}, {Ten, Hearts}, {Nine, Hearts}};

        EXPECT(compareHands(handStraight,handRoyalFlush) == "Player 2 wins with Royal Flush");
        EXPECT(compareHands(handStraightFlush,handTwoPairs)=="Player 1 wins with Straight Flush");
        EXPECT(compareHands(handFourOfAKind,handThreeOfAKind)=="Player 1 wins with Four of a kind");
        EXPECT(compareHands(handFullHouse,handThreeOfAKind)=="Player 1 wins with Full house");
        EXPECT(compareHands(handFlush,handThreeOfAKind)=="Player 1 wins with Flush");
        EXPECT(compareHands(handThreeOfAKind,handStraight)=="Player 2 wins with Straight");
        EXPECT(compareHands(handThreeOfAKind,handHighCard)=="Player 1 wins with Three of a kind");
        EXPECT(compareHands(handTwoPairs,handHighCard)=="Player 1 wins with Two pairs");
        EXPECT(compareHands(handOnePair,handHighCard)=="Player 1 wins with Pair");
        EXPECT(compareHands(handHighCard,handHighCard)=="Draw!");
    };
    TEST(checkHandFunctionWorksCorrectly)
    {   std::vector<Card> handRoyalFlush{{Ace, Hearts}, {King, Hearts}, {Queen, Hearts}, {Jack, Hearts}, {Ten, Hearts}};
        std::vector<Card> handStraightFlush{{Nine, Hearts}, {King, Hearts}, {Queen, Hearts}, {Jack, Hearts}, {Ten, Hearts}};
        std::vector<Card> handFourOfAKind{{Ace, Hearts}, {Ace, Hearts}, {Ace, Hearts}, {Ace, Hearts}, {Ten, Hearts}};
        std::vector<Card> handFullHouse{{Ace, Hearts}, {King, Hearts}, {Ace, Hearts}, {King, Hearts}, {King, Hearts}};
        std::vector<Card> handFlush{{Two, Hearts}, {King, Hearts}, {Ace, Hearts}, {King, Hearts}, {King, Hearts}};
        std::vector<Card> handStraight{{Nine, Spades}, {King, Clubs}, {Queen, Clubs}, {Jack, Clubs}, {Ten, Clubs}};
        std::vector<Card> handThreeOfAKind{{Ace, Hearts}, {King, Clubs}, {Three, Clubs}, {Three, Hearts}, {Three, Hearts}};
        std::vector<Card> handTwoPairs{{Ace, Hearts}, {King, Clubs}, {Ace, Clubs}, {King, Hearts}, {Nine, Hearts}};
        std::vector<Card> handOnePair{{Ace, Hearts}, {King, Clubs}, {Ace, Clubs}, {Ten, Hearts}, {Nine, Hearts}};
        std::vector<Card> handHighCard{{Ace, Hearts}, {King, Clubs}, {Three, Clubs}, {Ten, Hearts}, {Nine, Hearts}};

        EXPECT(checkHand(handRoyalFlush) == 1);
        EXPECT(checkHand(handStraightFlush) == 2);
        EXPECT(checkHand(handFourOfAKind) == 3);
        EXPECT(checkHand(handFullHouse) == 4);
        EXPECT(checkHand(handFlush) == 5);
        EXPECT(checkHand(handStraight) == 6);
        EXPECT(checkHand(handThreeOfAKind) == 7);
        EXPECT(checkHand(handTwoPairs) == 8);
        EXPECT(checkHand(handOnePair) == 9);
        EXPECT(checkHand(handHighCard) == 10);
    };

    TEST(mappingWorksCorrectly){
        std::vector<Rank> testMappingHand{Nine, King, Queen, Jack, King};
        auto cardCounts = makeCounts(testMappingHand);
        ASSERT(cardCounts.at(1) == 2);
        ASSERT(cardCounts.at(2) == 1);
        ASSERT(cardCounts.at(3) == 1);
        ASSERT(cardCounts.at(5) == 1);
        EXPECT(cardCounts.at(6) == 0);
    };

    TEST(isPairsWorksCorrectly)
    {
        std::vector<Card> handP{{Ace, Hearts}, {King, Clubs}, {Queen, Clubs}, {Ten, Hearts}, {Nine, Hearts}};
        std::vector<Card> handPTwo{{Ace, Hearts}, {King, Clubs}, {Ace, Clubs}, {Ten, Hearts}, {Nine, Hearts}};
        EXPECT(!isPair(handP));
        EXPECT(isPair(handPTwo));
    };
    TEST(isTwoPairsWorksCorrectly)
    {
        std::vector<Card> handTPTwo{{Ace, Hearts}, {King, Clubs}, {Ace, Clubs}, {King, Hearts}, {Nine, Hearts}};
        std::vector<Card> handTP{{Ace, Hearts}, {King, Clubs}, {Ace, Clubs}, {Ten, Hearts}, {Nine, Hearts}};
        EXPECT(!isTwoPairs(handTP));
        EXPECT(isTwoPairs(handTPTwo));
    };
    TEST(isThreeOfAKindWorksCorrectly)
    {
        std::vector<Card> handToaK{{Ace, Hearts}, {King, Clubs}, {Three, Clubs}, {Three, Hearts}, {Three, Hearts}};
        EXPECT(isThreeOfAKind(handToaK));
    };
    TEST(isStraightWorksCorrectly)
    {
        std::vector<Card> hand{{Nine, Spades}, {King, Clubs}, {Queen, Clubs}, {Jack, Clubs}, {Ten, Clubs}};
        std::vector<Card> handNoS{{Two, Spades}, {King, Spades}, {Queen, Spades}, {Jack, Spades}, {Ten, Hearts}};
        EXPECT(isStraight(hand));
        EXPECT(!isStraight(handNoS));
    };
        TEST(isFlushWorksCorrectly)
    {
        std::vector<Card> hand{{Nine, Clubs}, {King, Clubs}, {Queen, Clubs}, {Jack, Clubs}, {Ten, Clubs}};
        std::vector<Card> handNoF{{Two, Spades}, {King, Spades}, {Queen, Spades}, {Jack, Spades}, {Ten, Hearts}};
        EXPECT(isFlush(hand));
        EXPECT(!isFlush(handNoF));
    };
            TEST(isFullHouseWorksCorrectly)
    {
        std::vector<Card> handFullHouse{{Ace, Hearts}, {King, Hearts}, {Ace, Hearts}, {King, Hearts}, {King, Hearts}};
        EXPECT(isFullHouse(handFullHouse));
    };
        TEST(isFourOfAKindWorksCorrectly)
    {
        std::vector<Card> handFourOfAKind{{Ace, Hearts}, {Ace, Hearts}, {Ace, Hearts}, {Ace, Hearts}, {Ten, Hearts}};
        EXPECT(isFourOfAKind(handFourOfAKind));
    };
    TEST(isStraightFlushWorksCorrectly)
    {
        std::vector<Card> hand{{Nine, Clubs}, {King, Clubs}, {Queen, Clubs}, {Jack, Clubs}, {Ten, Clubs}};
        std::vector<Card> handNoRF{{Two, Spades}, {King, Spades}, {Queen, Spades}, {Jack, Spades}, {Ten, Hearts}};
        EXPECT(isStraightFlush(hand));
        EXPECT(!isStraightFlush(handNoRF));
    };
    TEST(isRoyalFlushWorksCorrectly)
    {
        std::vector<Card> hand{{Ace, Hearts}, {King, Hearts}, {Queen, Hearts}, {Jack, Hearts}, {Ten, Hearts}};
        std::vector<Card> handNoRF{{Two, Hearts}, {King, Hearts}, {Queen, Hearts}, {Jack, Hearts}, {Ten, Hearts}};
        EXPECT(isRoyalFlush(hand));
        EXPECT(!isRoyalFlush(handNoRF));
    };
}
