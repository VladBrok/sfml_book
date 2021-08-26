#pragma once
#include <unordered_map>
#include <memory>
#include <stdexcept>
#include <cassert>


template <typename Resource, typename Identifier>
class ResourceHolder
{
public:
    void                                                      load(const Identifier& id, 
                                                                   const std::string& filename);                                                      
    template <typename Parameter>
    void                                                      load(const Identifier& id, 
                                                                   const std::string& filename, 
                                                                   const Parameter& secondParam);
                                                              
    Resource&                                                 get(const Identifier& id) const;
     

private:                                                      
    void                                                      insertResource(const Identifier& id,
                                                                             std::unique_ptr<Resource> resource);


private:

    std::unordered_map<Identifier, std::unique_ptr<Resource>> mResourceMap; 
};

#include "ResourceHolder.inl"
