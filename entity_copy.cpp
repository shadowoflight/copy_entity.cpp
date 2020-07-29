#include "entity_factory.h"

#include "core/event_manager.h"

#include "component.h"
#include "entity.h"
#include "system.h"

#include "components/debug_component.h"
#include "components/hierarchy_component.h"
#include "components/music_component.h"
#include "components/physics/box_collider_component.h"
#include "components/physics/sphere_collider_component.h"
#include "components/script_component.h"
#include "components/short_music_component.h"
#include "components/test_component.h"
#include "components/transform_component.h"
#include "components/transform_animation_component.h"
#include "components/visual/camera_component.h"
#include "components/visual/cpu_particles_component.h"
#include "components/visual/directional_light_component.h"
#include "components/visual/point_light_component.h"
#include "components/visual/spot_light_component.h"
#include "components/visual/text_ui_component.h"
#include "components/visual/grid_model_component.h"
#include "components/visual/model_component.h"
#include "components/visual/ui_component.h"
#include "components/trigger_component.h"
#include "systems/hierarchy_system.h"

#define REGISTER_COMPONENT(ComponentClass)                                                            \
	m_ComponentCreators.push_back({ ComponentClass::s_ID, #ComponentClass, ComponentClass::Create }); \
	m_DefaultComponentCreators.push_back({ ComponentClass::s_ID, #ComponentClass, ComponentClass::CreateDefault })

EntityID EntityFactory::s_CurrentID = ROOT_ENTITY_ID;
EntityID EntityFactory::s_CurrentEditorID = -ROOT_ENTITY_ID;

void EntityFactory::RegisterAPI(sol::state& rootex)
{
	sol::usertype<EntityFactory> entityFactory = rootex.new_usertype<EntityFactory>("EntityFactory");
	entityFactory["Create"] = [](TextResourceFile* t) { return EntityFactory::GetSingleton()->createEntity(t); };
	entityFactory["Find"] = [](EntityID e) { return EntityFactory::GetSingleton()->findEntity(e); };
}

EntityFactory* EntityFactory::GetSingleton()
{
	static EntityFactory singleton;
	return &singleton;
}

EntityID EntityFactory::getNextID()
{
	return ++s_CurrentID;
}

EntityID EntityFactory::getNextEditorID()
{
	return --s_CurrentEditorID;
}





Ref<Entity> copyEntity(ID id,bool isEditorOnly)
{
   
for(auto &x:EntityFactory:: findEntity(id)-> getComponents<Hierarchy Components>()-> getChildren())
{
  
  copyEntity(x);
  return getEntityInfo(id,isEditorOnly);
}

}



Ref<Entity> getEntityInfo(ID id, bool isEditorOnly);
{
 if(id==0)
{
ERR("Entity is invalid");
return nullptr;
}
JSON::json name=findEntity(id)->getFullName();
JSON::josn componentsJSON=findEntity(id)->getComponents();

if(componentJSON.is_null())
{
ERR("Components not found while copying Entity")
return nullptr;
}
Ref<Entity> entity;
 EntityID newID=0;
if(isEditorOnly)
{
 newID=getNextEditorID();
}
else
{
newId=getNextID();
}
entity.reset(new Entity(newID, name.is_null() ? "Entity" : name));

	for (auto&& [componentName, componentDescription] : componentJSON.items())
	{
		Ref<Component> componentObject = createComponent(componentName, componentDescription);
		if (componentObject)
		{
			entity->addComponent(componentObject);
			componentObject->setOwner(entity);
		}
	}

	if (!entity->setupComponents())
	{
		ERR("Entity was not setup properly: " + std::to_string(entity->m_ID));
	}

	entity->setEditorOnly(isEditorOnly);

	m_Entities[entity->m_ID] = entity;

	PRINT("Copyied entity: " + entity->getFullName()+entity->getID());

	return entity;
}
