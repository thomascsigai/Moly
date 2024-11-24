#pragma once

#include <Event.h>

namespace Moly
{
	class MOLY_API KeyEvent : public Event
	{
	public:
		int key;
		bool pressed;

		KeyEvent(int key, bool pressed) : key(key), pressed(pressed) {}

		std::string GetEventType() const override { return "KeyEvent"; }
	};
}