/*
 * RocketEventListener.cpp
 *
 *  Created on: Jan 9, 2016
 *      Author: nathan
 */

#include "RocketEventListener.h"

#include <Rocket/Core/Factory.h>
#include <Core/Context.h>

#include <IO/Log.h>
RocketEventListenerInstancer::RocketEventListenerInstancer(Urho3D::Context* context): Object(context), executeAngelScript_(true), executeLuaScript_(true), sendUrhoEvents_(true)
{
	// TODO Auto-generated constructor stub

}

RocketEventListenerInstancer::~RocketEventListenerInstancer()
{
	// TODO Auto-generated destructor stub
}

void RocketEventListenerInstancer::Register(Urho3D::Context* context)
{
	context->RegisterSubsystem(new RocketEventListenerInstancer(context));
	Factory::RegisterEventListenerInstancer(context->GetSubsystem<RocketEventListenerInstancer>());
}

/// Instance an event listener object.
/// @param value Value of the event.
/// @param element Element that triggers the events.
EventListener* RocketEventListenerInstancer::InstanceEventListener(const String& value, Element* element)
{
//	LOGERROR("Instanced event listerner." + Urho3D::String((intptr_t)GetSubsystem<RocketEventListener>()) + value.CString() + element->GetClassNames().CString());
//	if (GetSubsystem<RocketEventListener>())
//	{
//		element->AddEventListener(value,GetSubsystem<RocketEventListener>(),true);
//		return GetSubsystem<RocketEventListener>();
//	}
//
//	GetContext()->RegisterSubsystem(this);
//	return this;
	return new RocketEventListener(value, this);
}

Urho3D::Variant ToVariant(Variant* v)
{
	if (!v)
		return Urho3D::Variant::EMPTY;
	switch(v->GetType())
	{
	case Variant::INT:
		return Urho3D::Variant(v->Get<int>());
	case Variant::BYTE:
		return Urho3D::Variant(v->Get<byte>());
	case Variant::CHAR:
		return Urho3D::Variant(v->Get<char>());
	case Variant::FLOAT:
		return Urho3D::Variant(v->Get<float>());
	case Variant::SCRIPTINTERFACE:
		return Urho3D::Variant((void*)v->Get<ScriptInterface*>());
	case Variant::STRING:
		return Urho3D::Variant(v->Get<String>().CString());
	case Variant::VECTOR2:
	{
		Urho3D::Vector2 ve(v->Get<Vector2f>().x,v->Get<Vector2f>().y);
		return Urho3D::Variant(ve);
	}
	case Variant::VOIDPTR:
		return Urho3D::Variant(v->Get<void*>());
	case Variant::WORD:
		return Urho3D::Variant(v->Get<word>());
	case Variant::NONE:
	default:
		return Urho3D::Variant::EMPTY;
	}
}

Urho3D::VariantMap ToVariantMap(const Dictionary* d)
{
	if (!d)
		return Urho3D::VariantMap();
	int i = 0;
	String key;
	Variant* v;
	Urho3D::VariantMap map;
	while (i <= d->Size())
	{
		d->Iterate(i,key,v);
		map[key.CString()] = ToVariant(v);
	}
	return map;
}
/// Releases this event listener instancer.
void RocketEventListenerInstancer::Release()
{
//	GetContext()->RemoveSubsystem<RocketEventListener>();
}

void RocketEventListenerInstancer::Execute(EventListener* l, Event& event)
{
	if (executeAngelScript_)
	{

	}
	if (executeLuaScript_)
	{

	}
	if (sendUrhoEvents_)
	{
		Urho3D::String name(event.GetType().CString());

//		//Print Event data
//		Urho3D::String phase((event.GetPhase() == Event::PHASE_BUBBLE) ? "bubble" : (event.GetPhase() == Event::PHASE_TARGET) ? "target" : "capture");
//		LOGRAW(Urho3D::ToString("%s_%s @ %d: ", name.CString(), phase.CString(), event.GetCurrentElement()));
//		int i = 0;
//		String key;
//		Variant* v;
//		while (i<= event.GetParameters()->Size())
//		{
//			event.GetParameters()->Iterate(i,key,v);
//			LOGRAW(Urho3D::ToString("\n\t%s=%c", key.CString(), v ? v->GetType() : ' '));
//		}
//		LOGRAW("\n");

		//Send the event to be handled by Urho Components
		Urho3D::VariantMap eventData = ToVariantMap(event.GetParameters());
		eventData["Element"] = (void *) event.GetCurrentElement();
		eventData["Phase"] = (int) event.GetPhase();
		eventData["Event"] = name;
		SendEvent("RocketUIEvent", eventData);
		if (eventData.Contains("StopPropagation"))
			if (eventData["StopPropagation"].GetBool() || eventData["StopPropagation"].GetInt())
				event.StopPropagation();
	}
}
/// Process the incoming Event


RocketEventListener::RocketEventListener(const String& str, RocketEventListenerInstancer* parent): eventCall_(str.CString()), parent_(parent)
{
	// TODO Auto-generated constructor stub

}

RocketEventListener::~RocketEventListener()
{
	// TODO Auto-generated destructor stub
}

void RocketEventListener::ProcessEvent(Event& event)
{
	parent_->Execute(this, event);
}

/// Called when the listener has been attached to a new Element
void RocketEventListener::OnAttach(Element* element)
{
	if (!element)
		return;
	LOGINFO(Urho3D::String("Attaching to element ") + element->GetTagName().CString());
}

/// Called when the listener has been detached from a Element
void RocketEventListener::OnDetach(Element* element)
{
	if (!element)
		return;
	LOGINFO(Urho3D::String("Detaching from element ") + element->GetTagName().CString());
	delete this;
}
