#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>
#include <vector>
using namespace std;


int main() {
    vector<string> v;
    split(v, "asgdjs=hjasf=gaiusfgi==ashjgfguyqw=12489", boost::algorithm::is_any_of("="));
    for (auto x : v) cout << x << endl;
    // for (int i = 0; i < v.size(); i++) {
    //     cout << v[i] << endl;
    // }

    return 0;
}