#ifndef User_Defined
#define User_Defined

#include <string>
#include <vector>
#include <set>

using namespace std;

struct UserReview {
    int playerID;
    float rating;
};

#define MAX_REVIEWS 20


class User {
    public :
        int id;
        vector<UserReview> reviews;
        void addReview(UserReview review);
        void getReviews(vector<UserReview>* outVector);
        User(int id);
};

User::User(int uid) {
    id = uid;
}

void User::addReview(UserReview review) {
    reviews.push_back(review);
    /*if (reviews.empty()) {
        reviews.push_back(review);
    } else {
        int insertionIndex = 0, reviewsSize = reviews.size();
        while (insertionIndex < reviewsSize && reviews[insertionIndex].rating < review.rating) {
            insertionIndex++;
        }
        reviews.insert(reviews.begin() + insertionIndex, review);
    }*/
}

void User::getReviews(vector<UserReview>* outVector) {
    for (int i = 0; i < reviews.size() && i < MAX_REVIEWS; i++)
        outVector->push_back(reviews[i]);
}

bool operator==(const User& u1, const User& u2) {
    return u1.id == u2.id;
}

bool operator==(const User& u1, const int& id) {
    return u1.id == id;
}

bool operator==(const int& id, const User& u1) {
    return u1.id == id;
}

#endif