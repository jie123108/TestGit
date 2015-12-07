#include <execinfo.h>
#include <iostream>
#include <stdlib.h>

using namespace std;
#define maxBackTraceFrames 24

void printStackTrace( std::ostream& os =std::cout) {
    
    void *b[maxBackTraceFrames];
    
    int size = ::backtrace( b, maxBackTraceFrames );
    for ( int i = 0; i < size; i++ )
        os << std::hex << b[i] << std::dec << '  ';
    os << std::endl;
    
    char **strings;
    
    strings = ::backtrace_symbols( b, size );
    for ( int i = 0; i < size; i++ )
        os << '#' << strings[i] << '\n';
    os.flush();
    free( strings );
}

int test2()
{
    printStackTrace(std::cout);
    return 0;
}

int test()
{
    return test2();
}


int main(){

    test();
    return 0;
}
