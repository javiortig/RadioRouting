#include "Message.h"

using namespace std;
/*
    {len;id;CMD=VALUE;CMD=VALUE}
*/

int main()
{
    const char *c = "{30;PP;/pozo=FALSE;/door=open}";
    String s = c;
    cout << s.length() << endl;

    Message m(c);

    m.print();
    return 0;
}
