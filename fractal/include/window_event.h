#ifndef WINDOW_EVENT_H
#define WINDOW_EVENT_H

namespace Fractal {	
    struct QuitEvent : public Event {
		QuitEvent()
			: Event("Quit Event") { }
		virtual ~QuitEvent() = default;

		const char* name() const { return m_name; }
	};

	struct ResizeEvent : public Event {
		ResizeEvent(int width, int height)
			: Event("Resize Event"), m_width(width), m_height(height) { }
		virtual ~ResizeEvent() = default;

		const char* name() const { return m_name; }

		int m_width, m_height;
	}; 
} // namespace Fractal


#endif // !WINDOW_EVENT_H