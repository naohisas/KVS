#include "FeatureTag.h"


namespace kvs
{

namespace bdml
{

FeatureTag::FeatureTag(): Tag("feature")
{
    m_featureName = "";
    m_featureScale = 0.0f;
    m_featureUnit = "";
}

void FeatureTag::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Feature Name: " << m_featureName << std::endl;
    os << indent << "Feature Scale: " << m_featureScale << std::endl;
    os << indent << "Feature Unit: " << m_featureUnit << std::endl;
}

bool FeatureTag::read( const Node* parent )
{
    if ( !Tag::read( parent ) ) { return false; }

    Tag featureName_tag("featureName");
    if ( featureName_tag.read( this->node() ) ) { m_featureName = NodeValue<std::string>( featureName_tag.node() ); }

    Tag featureScale_tag("featureScale");
    if ( featureScale_tag.read( this->node() ) ) { m_featureScale = NodeValue<float>( featureScale_tag.node() ); }

    Tag featureUnit_tag("featureUnit");
    if ( featureUnit_tag.read( this->node() ) ) { m_featureUnit = NodeValue<std::string>( featureUnit_tag.node() ); }

    return true;
}

} // end of namespace bdml

} // end of namespace kvs
