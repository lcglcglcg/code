#include <list> 
#include <iostream> 
using namespace std;

bool compfn(pair<int,int>  &x,pair<int,int>  &y)  
{
    if(x.first>=y.first)   
	return   true;   
    else   
	return   false;   

}//手工版的比较函数

int main(int argc, char **argv)
{



    list<pair<int,int> > lst;
    lst.push_back(make_pair(1,1));
    lst.push_back(make_pair(200,1));
    lst.push_back(make_pair(30,1));
    lst.sort(compfn);

    cout<<(lst.begin())->first;

    return 0;
}
