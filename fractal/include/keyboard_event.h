#ifndef KEYBOARD_EVENT_H
#define KEYBOARD_EVENT_H

#include "event.h"

namespace Fractal {
    struct KeyboardEvents : public Event {
    KeyboardEvents(int key, int scancode, int action)
        : Event("Keyboard Event"), m_key(key), m_scancode(scancode), m_action(action) {  }
    virtual ~KeyboardEvents() = default;

    const char* name() const { return m_name; }

    int m_key;
    int m_scancode;
    int m_action;
};  
} // namespace Fractal

#endif // !KEYBOARD_EVENT_H