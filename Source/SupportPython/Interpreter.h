#pragma once


namespace kvs
{

namespace python
{

class Interpreter
{
public:
    Interpreter( const bool import = true );
    ~Interpreter();

    void initialize( const bool import = true );
    void finalize();
    bool isInitialized() const;
};

} // end of namespace python

} // end of namespace kvs
