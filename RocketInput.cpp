/*
 * RocketInput.cpp
 *
 *  Created on: Jan 9, 2016
 *      Author: nathan
 */

#include "RocketInput.h"
#include <Urho3D.h>
#include <Core/Context.h>

using namespace Urho3D;

RocketInput::RocketInput(Context* context, ::Rocket::Core::Context* rContext): Object(context), _rocketContext(rContext)
{
	// TODO Auto-generated constructor stub
	SubscribeToEvent(Urho3D::E_KEYUP, HANDLER(RocketInput, HandleInput));
	SubscribeToEvent(Urho3D::E_KEYDOWN, HANDLER(RocketInput, HandleInput));
	SubscribeToEvent(Urho3D::E_MOUSEBUTTONDOWN, HANDLER(RocketInput, HandleInput));
	SubscribeToEvent(Urho3D::E_MOUSEBUTTONUP, HANDLER(RocketInput, HandleInput));
	SubscribeToEvent(Urho3D::E_MOUSEMOVE, HANDLER(RocketInput, HandleInput));
	SubscribeToEvent(Urho3D::E_MOUSEWHEEL, HANDLER(RocketInput, HandleInput));
	SubscribeToEvent(Urho3D::E_TEXTINPUT, HANDLER(RocketInput, HandleInput));

}

RocketInput::~RocketInput()
{
	// TODO Auto-generated destructor stub
	_rocketContext.reset();
}


#include <Rocket/Core/Input.h>
#include <SDL/SDL_keycode.h>
::Rocket::Core::Input::KeyIdentifier GetKey(int SDL_keycode)
{
//	switch (key)
//	{
//	case SDLK_0
//	}
	using namespace ::Rocket::Core::Input;
	if (SDL_keycode >= SDLK_0 && SDL_keycode <= SDLK_9)
		return KeyIdentifier(SDL_keycode - SDLK_0 + KI_0);
	if (SDL_keycode >= SDLK_a && SDL_keycode <= SDLK_z)
		return KeyIdentifier(SDL_keycode - SDLK_a + KI_A);
	if (SDL_keycode >= SDLK_F1 && SDL_keycode <= SDLK_F24)
		return KeyIdentifier(SDL_keycode - SDLK_1 + KI_1);
	if (SDL_keycode == SDLK_SPACE)
		return KI_SPACE;
	if (SDL_keycode == SDLK_UNKNOWN)
		return KI_UNKNOWN;
	switch (SDL_keycode)
	{
	case SDLK_SPACE:
		return KI_SPACE;
	case SDLK_BACKSPACE:
		return KI_BACK;
	case SDLK_TAB:
		return KI_TAB;
	case SDLK_ESCAPE:
		return KI_ESCAPE;
	case SDLK_RETURN:
		return KI_RETURN;
	case SDLK_UP:
		return KI_UP;
	case SDLK_DOWN:
		return KI_DOWN;
	case SDLK_LEFT:
		return KI_LEFT;
	case SDLK_RIGHT:
		return KI_RIGHT;
	case SDLK_PAGEUP:
		return KI_PRIOR;
	case SDLK_PAGEDOWN:
		return KI_NEXT;
	case SDLK_END:
		return KI_END;
	case SDLK_HOME:
		return KI_HOME;
	case SDLK_INSERT:
		return KI_INSERT;
	case SDLK_DELETE:
		return KI_DELETE;
	default:
		return KI_UNKNOWN;
	}

}

int GetKeyModifier(int mod)
{
	using namespace ::Rocket::Core::Input;
	int out = 0;
	if (mod & KMOD_CTRL)
		out |= KM_CTRL;
	if (mod & KMOD_SHIFT)
		out |= KM_SHIFT;
	if (mod & KMOD_ALT)
		out |= KM_ALT;
	if (mod & KMOD_GUI)
		out |= KM_META;
	if (mod & KMOD_CAPS)
		out |= KM_CAPSLOCK;
	if (mod & KMOD_NUM)
		out |= KM_NUMLOCK;
//	if (mod & KMOD_)//scroll lock
//		out != KM_;
	return out;
}

void RocketInput::HandleInput(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{

	int mod = GetKeyModifier(GetSubsystem<Input>()->GetQualifiers());
	if (eventType == E_KEYUP)
		_rocketContext->ProcessKeyUp(GetKey(eventData[KeyUp::P_KEY].GetInt()), mod);
	else if (eventType == E_KEYDOWN)
		_rocketContext->ProcessKeyDown(GetKey(eventData[KeyUp::P_KEY].GetInt()), mod);
	else if (eventType == E_TEXTINPUT)
		_rocketContext->ProcessTextInput(eventData[TextInput::P_TEXT].GetString().CString());
	else if (eventType == E_MOUSEMOVE)
		_rocketContext->ProcessMouseMove(eventData[MouseMove::P_X].GetInt(),eventData[MouseMove::P_Y].GetInt(), mod);
	else if (eventType == E_MOUSEBUTTONUP)
		_rocketContext->ProcessMouseButtonUp(eventData[MouseButtonUp::P_BUTTON].GetInt() - 1,mod);
	else if (eventType == E_MOUSEBUTTONDOWN)
		_rocketContext->ProcessMouseButtonDown(eventData[MouseButtonDown::P_BUTTON].GetInt() - 1,mod);
	else if (eventType == E_MOUSEWHEEL)
		_rocketContext->ProcessMouseWheel(eventData[MouseWheel::P_WHEEL].GetInt(),mod);

}
