
namespace util
{


template <typename T>
std::string toString(const T& value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}


template <typename T>
void centerOrigin(T& object)
{
    sf::FloatRect bounds(object.getLocalBounds());
    object.setOrigin(std::floor(bounds.left + bounds.width / 2.f), 
                     std::floor(bounds.top + bounds.height / 2.f));
}


} // namespace util
