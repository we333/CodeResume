#include <iostream>
#include <string>
#include <ext/hash_map>

using namespace std;

namespace __gnu_cxx
{
        template<> struct hash< std::string >
        {
                size_t operator()( const std::string& x ) const
                {
                        return hash< const char* >()( x.c_str() );
                }
        };
}

using __gnu_cxx::hash_map;

void GetMaxFrequencyString_SingleThread( vector<string>& vectorA, vector<string>& vectorB )
{
	// Let's write your code !

	hash_map<string, int> table;
	int size = vectorA.size();
    
    for(int i = 0; i < size; i++)
    {
        table[vectorA[i]]++;
    }
    
    hash_map<string, int>::iterator begin = table.begin(); 
    hash_map<string, int>::iterator end = table.end(); 
	hash_map<string, int>::iterator it; 
	
    int max = 0;
    for(it = begin; it != end; it++)
    {
    	if((*it).second > max)
            max = (*it).second;
    }
cout<<"max = "<<max<<endl;
    for(it = begin; it != end; it++)
    {
    	if((*it).second == max)
        {
            vectorB.push_back((*it).first);
        }
    }

    for(int i = 0; i < vectorB.size(); i++)
    	cout<<vectorB[i]<<' ';
    cout<<endl;
}

string List[] = {"Michael","Jack", "Michael","Michael","Damon","Damon", "Damon"};

int main()
{
	vector<string> A, B;

	for(int i = 0; i < sizeof(List)/sizeof(List[0]); i++)
		A.push_back(List[i]);

	GetMaxFrequencyString_SingleThread(A, B);
	return 0;
}