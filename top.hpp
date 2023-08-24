#ifndef Top_Defined
#define Top_Defined

#include <string>
#include <vector>
#include <set>

using namespace std;







void topPlayers(vector table, int n){

  cout << "Top " << n << " jogadores:" << endl;

  int topn;

  auto fun = [](Player p1, Player p2)
  {
    return (p1.totalRating / p1.ratingCount) > (p2.totalRating / p2.ratingCount);
  };

  std::set<Player, decltype(fun)> topList(fun);
  float pscore;
  float prevbest = 0;

  for (int pindex = 0; pindex < table.elements.size(); pindex++)
  {
    for (int sindex = 0; sindex < table.elements[pindex].size(); sindex++)
    {
      if (table.elements[pindex][sindex].ratingCount >= 1000)
      {
        pscore = table.elements[pindex][sindex].totalRating / table.elements[pindex][sindex].ratingCount;

         topList.insert(table.elements[pindex][sindex]);

      }
    }
  }
  set<Player, decltype(fun)>::iterator iterator;
  int i = 0;
  for(iterator = topList.begin(); i < topn && iterator != topList.end(); iterator++){
     cout << iterator->name << " -> " << iterator->totalRating / iterator->ratingCount << endl;
     i++;
  }


}

#endif