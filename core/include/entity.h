#ifndef _H_CORE_ENTITY_H_
#define _H_CORE_ENTITY_H_
#include <cstdint>
#include <vector>
#include <unordered_map>
namespace core{
namespace ems{



class Entity{
public:
  Entity(uint64_t id, uint64_t componentMask){
    m_id = id;
    m_componentMask = componentMask;
  }

  uint64_t id() const {
    return m_id;
  }

  uint64_t mask() const{
    return m_componentMask;
  }

  bool hasComponent(uint64_t componentId){
    uint64_t mask = 1 << componentId;
    uint64_t bit  = mask & m_componentMask;
    return bit;
  }

private:
  uint64_t m_id;
  uint64_t m_componentMask;
};

template<uint32_t TYPE_ID,typename _T_Type>
class Component{
public:
  static constexpr uint64_t MaskBit = 1 << TYPE_ID;
  static constexpr uint64_t TypeId  = TypeId;
  
  uint64_t id() const {
    return TypeId; 
  }

  uint64_t bitMask() const {
    return MaskBit;
  }
  
  uint32_t conditionalCreate(uint32_t type, const _T_Type& c = {}){
    if(type == TypeId){
      return create(c); 
    }
    return 0;
  }

  uint32_t create(const _T_Type& c = {}){
    uint32_t id   = m_id++;
    uint32_t indx = m_components.size();
    m_componentIdMap[id] = indx;
    m_components.push_back(c);
    return id;
  }

  bool destroy(uint32_t){
    return true;
  }

  _T_Type& operator[](const uint32_t& indx){
    return m_components[m_componentIdMap[indx]];
  }

  auto begin(){
    return m_components.begin();  
  }

  auto end(){
    return m_components.end();
  }

  auto cbegin(){
    return m_components.cbegin();
  }

  auto cend(){
    return m_components.cend();
  }

private:
  std::vector<_T_Type>                  m_components;
  std::unordered_map<uint32_t,uint32_t> m_componentIdMap;
  uint32_t                              m_id = 0;
};

template<typename _T_Entity,typename ..._T_Components>
struct EntityManagerDesc{
  typedef _T_Entity Entity;
  typedef std::tuple<_T_Components...> ComponentList; 
};


template<typename _T_Desc>
class EntityManager{
public:
  typedef typename _T_Desc::Entity EntityType;
  typedef typename _T_Desc::ComponentList SupportedComponents;
  
  EntityType createEntity( const std::vector<uint32_t>& components ){
    uint64_t componentMask = 0;
    uint64_t id            = m_id++;

    for(const uint32_t& c : components){
      componentMask |= ( 1 << c );
      std::apply([&](const auto&&... a){
        uint32_t componentId = a.conditionalCreate(c) + ...;
      },m_components);
    }
    EntityType entity(id,componentMask);
    return entity;
  }
  
  template<typename T>
  auto component(){
    return std::get<T>(m_components);
  }

private:
  typename SupportedComponents::ComponentList m_components;
  uint64_t m_id = 0;
};


};
}
#endif
