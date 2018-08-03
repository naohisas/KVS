#include "PropertyTag.h"

namespace kvs
{

namespace bdml
{

PropertyTag::PropertyTag(): Tag("property")
{
    m_featureRef = "";
    m_featureVal = 0.0f;
}

void PropertyTag::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Feature Ref.: " << m_featureRef << std::endl;
    os << indent << "Feature Val.: " << m_featureVal << std::endl;
}

bool PropertyTag::read( const Node* parent )
{
    if ( !Tag::read( parent ) ) { return false; }

    Tag featureRef_tag("featureRef");
    if ( featureRef_tag.read( this->node() ) ) { m_featureRef = NodeValue<std::string>( featureRef_tag.node() ); }

    Tag featureVal_tag("featureVal");
    if ( featureVal_tag.read( this->node() ) ) { m_featureVal = NodeValue<float>( featureVal_tag.node() ); }

    return true;
}

} // end of namespace bdml

} // end of namespace kvs
