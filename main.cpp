#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
using namespace std;

struct intersections;

struct connections
{  
  /*The roads (and other things) that run between those intersections, in no particular
    order. Each line contains five space-free items:

      the official name of the road, or "?" if it hasn't got one
      what kind of road it is, as a three character code
          the only really significant part of this is the first letter - F indicates
          that it is a ferry crossing and not a road at all.
      intersection A
      intersection B
          the road connects A to B. A and B are line numbers (counting from 0) in
          the intersections.txt file
      the length of this section of road, in miles. */

  string roadName;
  string roadType;
  intersections * intersectionA;
  intersections * intersectionB;
  double length;

  connections(string rN, string rT, intersections * iA, intersections * iB, double l)
  {
    roadName = rN;
    roadType = rT;
    intersectionA = iA;
    intersectionB = iB;
    length = l;
  }
};

struct intersections
{   /*
    Everywhere in the U.S. that two major roads meet, or that one major road has
    a notable change of direction. One per line. The first line is considered to
    be line 0 and does not contain useful data.

      longitude
      latitude
      distance from nearest named place
      what that named place is, state then name.
    */
  double longtitude;
  double latitude;
  double distFromPlace;
  string placeState;
  string placeCity;
  int num;
  vector <connections*> Connections;

  intersections(double lo, double la, double dfp, string pS, string pC, int i)
  {
    longtitude = lo;
    latitude = la;
    distFromPlace = dfp;
    placeState = pS;
    placeCity = pC;
    num = i;
  }

  void printRoutes(int i)
  {
    cout<<'\n'<<"Location "<<i<<", "<< distFromPlace << " miles from"
        <<placeCity<<", "<<placeState<<'\n';
    cout<<"roads leading away:\n";
    for(int j = 1; j<=Connections.size(); j++)
    {
      cout<<" "<<j<<": "<<Connections[j-1]->roadName<<", "
          <<Connections[j-1]->length<<" miles to location ";
      if(Connections[j-1]->intersectionB->num==i)
        cout<<Connections[j-1]->intersectionA->num<<'\n';
      else
        cout<<Connections[j-1]->intersectionB->num<<'\n';
    }
    cout<<"take which road? ";
  }

  void addConnection(connections * road)
  {
    Connections.push_back(road);
  }
};




//read file connections
  /// one space between each data
  /// US-2 T-- 281 274 10.301
vector <connections*> readAndReturnConnections(vector<intersections*>&I)
{
  vector<connections*> connects;
  ifstream File;
  File.open("data/connections.txt");
  //File.open("/home/www/class/een318/connections.txt"); 
  if(File.fail())
  {
    cout<<"error reading connections file";
    exit(EXIT_FAILURE);
  }
  while(true)
  {
    string rN, rT;
    int from, to;
    double l;
    File>>rN>>rT>>from>>to>>l;
    if(File.fail())
      break;
    intersections *ia, *ib;
    ia = I[from];
    ib = I[to];
    connections *read = new connections(rN,rT,ia,ib,l);
    connects.push_back(read);
    I[from]->addConnection(read);
    I[to]->addConnection(read);
  }
  File.close();
  return connects;
}


//read file intersections
  /// -122.3135   47.2812    1.42 WA Lakeland South
  /// -123.8130   46.9763    0.27 WA Aberdeen
vector<intersections*> readAndReturnIntersections()
{
  vector<intersections*> inters;
  int i = 0;
  ifstream file;
  file.open("data/intersections.txt");
  //file.open("/home/www/class/een318/intersections.txt");
  if(file.fail())
  {
    cout<<"error reading intersections file";
    exit(EXIT_FAILURE);
  }
  while(true)
  {
    double lo, la, dfP;
    string pS, pC;
    file>>lo>>la>>dfP>>pS;
    getline(file,pC,'\n');
    if(file.fail())
      break;
    intersections *read = new intersections(lo,la,dfP,pS,pC,i);
    inters.push_back(read);
    i++; 
  }
  return inters;
}

intersections* grab(vector<intersections*>f)
{
  cout<<"Location to start: ";
  int i;
  cin>>i;
  f[i]->printRoutes(i);
  return f[i];
}

void print(vector<intersections*>I, intersections *i, int j)
{
  intersections* read;
  if(j==0 || j > i->Connections.size())
    return;
  else
  {
    if(i->Connections[j-1]->intersectionB->num==i->num)
      read = i->Connections[j-1]->intersectionA;
    else
      read = i->Connections[j-1]->intersectionB;
  }
  I[read->num]->printRoutes(read->num);
}

intersections* r(vector<intersections*>I, intersections* i, int j)
{
  intersections* read;
  if(j==0 || j>i->Connections.size())
    return NULL;
  else
  {
    if(i->Connections[j-1]->intersectionB->num==i->num)
      read=i->Connections[j-1]->intersectionA;
    else
      read=i->Connections[j-1]->intersectionB;
  }
  return I[read->num];
}


int main() 
{
  vector <intersections*> locations = readAndReturnIntersections();
  vector <connections*> roads = readAndReturnConnections(locations);
  intersections* location = grab(locations);
  while(true)
  {
    int n;
    cin>>n;
    print(locations, location, n);
    location = r(locations, location, n);
    if(location == NULL)
    { cout<<'\n'<<"Please enter a number that cooresponds to a road. Exited"
          <<'\n';
      break;
    }
  }
} 