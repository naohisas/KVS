#pragma once
#include <string>
#include <kvs/RGBAColor>
#include <kvs/Vector2>

struct FONScontext;


namespace kvs
{

class FontStash
{
public:
    enum
    {
        ZeroTopLeft = 1,
        ZeroBottomLeft = 2,
    };

    struct Metrics
    {
        float ascender;
        float descender;
        float lineh;
    };

    class Context
    {
    private:
        FONScontext* m_context;
    public:
        Context( int width, int height, int flag );
        ~Context();
        FONScontext* operator () () { return m_context; }
        FONScontext* operator () () const { return m_context; }
    };

private:
    Context m_context; ///< context

public:
    FontStash( const int width, const int height, const int flag );
    ~FontStash();

    void setSize( const float size );
    void setColor( const unsigned int color_id );
    void setSpacing( const float spacing );
    void setBlur( const float blur );
    void setAlign( const int align );
    void setFont( const int font_id );
    int addFont( const std::string& name, const std::string& source );
    int fontID( const std::string& name ) const;
    unsigned int colorID( const kvs::RGBAColor& color ) const;
    Metrics metrics() const;
    float ascender() const;
    float descender() const;
    float lineHeight() const;
    float textBounds( const kvs::Vec2& p, const std::string& text ) const;
    void clearState();
    void pushState();
    void popState();
    void draw( const kvs::Vec2& p, const std::string& text );
};

} // end of namespace kvs
