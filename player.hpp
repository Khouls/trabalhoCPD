#ifndef Player_Defined
#define Player_Defined

#include <string>
#include <vector>
#include <set>

struct Player{
    int id;
    std::string sofifaID;
    std::string name;
    float totalRating;
    int ratingCount;
};

bool operator==(const Player& p1, const Player& p2) {
    return p1.id == p2.id;
}

bool operator==(const int& idx, const Player& p) {
    return idx == p.id;
}

bool operator==(const Player& p, const int& idx) {
    return idx == p.id;
}

struct PlayerTuple {
    int playerID;
    int count;
};

bool operator==(const PlayerTuple& p1, const PlayerTuple& p2) {
    return p1.playerID == p2.playerID;
}

bool operator<(const PlayerTuple& p1, const PlayerTuple& p2) {
    return p1.playerID < p2.playerID;
}

struct TagTuple {
    std::string tag;
    std::vector<PlayerTuple>* players;
};

bool operator==(const TagTuple& t1, const TagTuple& t2) {
    return t1.tag.compare(t2.tag) == 0;
}

bool operator<(const TagTuple& t1, const TagTuple& t2) {
    return t1.tag.compare(t2.tag) == 0;
}

#endif