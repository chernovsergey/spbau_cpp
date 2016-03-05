#include <iostream>
#include "shared_buffer.h"
#include "lazy_string.h"

using namespace std;

int main()
{

//    Testing shared buffer;
//    shared_buffer b1(5);
//    shared_buffer b2(b1);
//    shared_buffer b3(5);
//    b3 = b2;

    auto seq = "ACGT-ACGT-ACGT";
    auto yeq = "PROT-EINP-ROTE";

    lazy_string s1(seq);

    lazy_string s2(s1);

    lazy_string s3(s1);
    s3.set_at(0, 'O');

    lazy_string s4 = s3;
    s4.set_at(0, 'X');

    lazy_string s5(yeq);
    size_t pos = s5.find("EINP", 0);

    lazy_string s6(seq);
    s6 += s5;

    lazy_string s7("");
    s7 = s5 + s6;

    char c = s7[0];

    s7[1] = 'x';

    cout << s1 << endl;
    cout << s2 << endl;
    cout << s3 << endl;
    cout << s4 << endl;
    cout << s5 << endl;
    cout << s6 << endl;
    cout << s7 << endl;
    cout << c  << endl;
    cout << (s6 < s7) << endl;
    cout << (s7 > s7) << endl;
    cout << "Substring 'EINP' at posigion " << pos << " of s5" << endl;
    return 0;
}
