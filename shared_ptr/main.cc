#include <iostream>
#include "shared_ptr.h"
#include <vector>
#include <memory>
#include "flyd_shared_ptr.h"

using namespace flyd;

int main()
{
    cout << "hello world" << endl;

    int *p = new int(10);
    mshared_ptr<int> mp(p);  //新建一个智能指针类
    flyd::shared_ptr<int>  sp(p);
    cout << "mp的计数为：" << mp.count() << endl;
    vector<mshared_ptr<int> > s;
    vector<flyd::shared_ptr<int> > ss;
    ss.push_back(sp);
    cout << "sp的计数为：" << sp.use_count() << endl;
    s.push_back(mp);
    cout << "mp的计数为：" << mp.count() << endl;
    



    return 0;
}