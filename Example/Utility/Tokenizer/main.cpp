#include <iostream>
#include <kvs/Tokenizer>


int main()
{
    kvs::Tokenizer tokenizer( "This is a sample string.", " " );
    while ( !tokenizer.isLast() )
    {
        std::cout << tokenizer.token() << std::endl;
    }

    kvs::Tokenizer tokenizer2( ":,  This, is a sample,  string.,  :", "," );
    while ( !tokenizer2.isLast() )
    {
        std::cout << tokenizer2.token() << std::endl;
    }

    kvs::Tokenizer tokenizer3( ":,  This, is a sample,  string.,  :", ", :" );
    while ( !tokenizer3.isLast() )
    {
        std::cout << tokenizer3.token() << std::endl;
    }

    return 0;
}
