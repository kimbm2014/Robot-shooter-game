

#include <ostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include <csignal>
#include <regex>
#include <type_traits>
#include <set>

using namespace std;

class streambuf_switcher
{
public:
    streambuf_switcher(ios& dest, ios& src)
    : dest_stream(dest), saved_streambuf(dest.rdbuf(src.rdbuf()))
    {}
    ~streambuf_switcher()
    {
        dest_stream.rdbuf(saved_streambuf);
    }
private:
    ios& dest_stream;
    streambuf* saved_streambuf;
};

set<void*> addrs;
bool recordaddrs = false;

void* operator new(size_t n)
{
    void* p = malloc(n);
    if (recordaddrs  &&  n == sizeof(Martian))
    {
        recordaddrs = false;
        addrs.insert(p);
        recordaddrs = true;
    }
    return p;
}

void operator delete(void* p) noexcept
{
    if (recordaddrs)
    {
        recordaddrs = false;
        set<void*>::iterator it = addrs.find(p);
        if (it != addrs.end())
            addrs.erase(it);
        recordaddrs = true;
    }
    free(p);
}

void operator delete(void* p, std::size_t s) noexcept
{
    s = addrs.size();   // these two lines do nothing other than
    s+=s;               // getting rid of unused var warning on g++
    if (recordaddrs)
    {
        recordaddrs = false;
        set<void*>::iterator it = addrs.find(p);
        if (it != addrs.end())
            addrs.erase(it);
        recordaddrs = true;
    }
    free(p);
}

void testone(int n)
{
    
    ostringstream strCout;
    streambuf_switcher sso(cout, strCout);
    istringstream in1("Edwin Ambrosio 9234\nMassoud Ghyam 8345\nSolomon Russell 7876\nSatish Singhal 3456");
    istringstream empty("");
    //Martian m; m.fname_ = "Joe"; m.lname_ = "Smith"; m.SSN_ = "55555";
    Martian m {"Joe", "Smith", "55555"};  // if you used a constructor for Martian I used this
    switch (n)
    {
        default: {
            cout << "Bad argument" << endl;
        } break; case  1: {
            MartianDatabase md(empty);
        } break; case  2: {
            MartianDatabase md(in1);
            md.printMartians(Criteria::first);
            string str = strCout.str();
            regex e("1[^[:w:]]+Edwin[^[:w:]]+Ambrosio");
            assert(regex_search(str, e));
            e = "3[^[:w:]]+Satish[^[:w:]]+Singhal";
            assert(regex_search(str, e));
        } break; case  3: {
            MartianDatabase md(in1);
            md.printMartians(Criteria::last);
            string str = strCout.str();
            regex e("1[^[:w:]]+Edwin[^[:w:]]+Ambrosio");
            assert(regex_search(str, e));
            e = "4[^[:w:]]+Satish[^[:w:]]+Singhal";
            assert(regex_search(str, e));
        } break; case  4: {
            MartianDatabase md(in1);
            md.printMartians(Criteria::ssn);
            string str = strCout.str();
            regex e("4[^[:w:]]+Edwin[^[:w:]]+Ambrosio");
            assert(regex_search(str, e));
            e =("1[^[:w:]]+Satish[^[:w:]]+Singhal");
            assert(regex_search(str, e));
        } break; case  5: {
            MartianDatabase md(in1);
            md.addMartian(m);
            md.printMartians(Criteria::first);
            string str = strCout.str();
            regex e("1[^[:w:]]+Edwin[^[:w:]]+Ambrosio");
            assert(regex_search(str, e));
            e =("3[^[:w:]]+Massoud[^[:w:]]+Ghyam");
            assert(regex_search(str, e));
            e =("4[^[:w:]]+Satish[^[:w:]]+Singhal");
            assert(regex_search(str, e));
        } break; case  6: {
            MartianDatabase md(in1);
            md.addMartian(m);
            md.printMartians(Criteria::last);
            string str = strCout.str();
            regex e("1[^[:w:]]+Edwin[^[:w:]]+Ambrosio");
            assert(regex_search(str, e));
            e =("2[^[:w:]]+Massoud[^[:w:]]+Ghyam");
            assert(regex_search(str, e));
            e =("5[^[:w:]]+Joe[^[:w:]]+Smith");
            assert(regex_search(str, e));
        } break; case  7: {
            MartianDatabase md(in1);
            md.addMartian(m);
            md.printMartians(Criteria::ssn);
            string str = strCout.str();
            regex e("5[^[:w:]]+Edwin[^[:w:]]+Ambrosio");
            assert(regex_search(str, e));
            e =("4[^[:w:]]+Massoud[^[:w:]]+Ghyam");
            assert(regex_search(str, e));
            e =("2[^[:w:]]+Joe[^[:w:]]+Smith");
            assert(regex_search(str, e));
        } break; case  8: {
            MartianDatabase md(in1);
            md.deleteMartian("8345");
            md.printMartians(Criteria::first);
            string str = strCout.str();
            regex e ("2[^[:w:]]+Satish[^[:w:]]+Singhal");
            assert(regex_search(str, e));
            e =("3[^[:w:]]+Solomon[^[:w:]]+Russell");
            assert(regex_search(str, e));
        } break; case  9: {
            MartianDatabase md(in1);
            md.deleteMartian("8345");
            md.printMartians(Criteria::last);
            string str = strCout.str();
            regex e ("3[^[:w:]]+Satish[^[:w:]]+Singhal");
            assert(regex_search(str, e));
            e =("2[^[:w:]]+Solomon[^[:w:]]+Russell");
            assert(regex_search(str, e));
        } break; case  10: {
            MartianDatabase md(in1);
            md.deleteMartian("8345");
            md.printMartians(Criteria::ssn);
            string str = strCout.str();
            regex e ("1[^[:w:]]+Satish[^[:w:]]+Singhal");
            assert(regex_search(str, e));
            e =("2[^[:w:]]+Solomon[^[:w:]]+Russell");
            assert(regex_search(str, e));
        } break; case  11: {
            MartianDatabase md(in1);
            assert(!md.deleteMartian("5555"));
        } break; case  12: {
            MartianDatabase md(in1);
            assert(md.deleteMartian("8345"));
            assert(md.deleteMartian("3456"));
            assert(md.deleteMartian("7876"));
            assert(md.deleteMartian("9234"));
            assert(md.isEmpty());
        } break; case  13: {
            MartianDatabase md(in1);
            assert(md.deleteMartian("8345"));
            assert(md.deleteMartian("3456"));
            assert(md.deleteMartian("7876"));
            assert(md.deleteMartian("9234"));
            assert(!md.isFull());
        } break; case  14: {
            recordaddrs = true;
            int oldn = addrs.size();
            {
                MartianDatabase md(in1);
                md.addMartian(m);
            }
            recordaddrs = false;
            assert(addrs.size() == oldn);
        } break; case  15: {
            recordaddrs = true;
            int oldn = addrs.size();
            {
                MartianDatabase md(in1);
                assert(md.deleteMartian("3456"));
            }
            recordaddrs = false;
            assert(addrs.size() == oldn);
        } break;
    }
}

int main()
{
    cout << "Enter test number: ";
    int n;
    cin >> n;
    testone(n);
    cout << "Passed" << endl;
}


 
 




