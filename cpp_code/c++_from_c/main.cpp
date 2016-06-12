#ifdef __cplusplus

extern "C" { 
#endif
    int add(int a, int b);
    int sub(int a, int b);
#ifdef __cplusplus
}

#endif

#include <iostream>

using namespace std;

int main()
{
    cout<<add(3, 2)<<endl;
    cout<<sub(3, 2)<<endl;
    return 0;
}
