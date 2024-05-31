/**
 * @file application.cpp
 * @author strah19
 * @date May 29 2024
 * @version 1.0
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT License as
 * published by the Free Software Foundation.
 *
 * @section DESCRIPTION
 *
 * This file contains the main program application code controlling
 * application setup, run, and clean up.
 */

#include "application.h"
#include "log.h"
#include "renderer_commands.h"
#include "utility.h"

namespace Fractal {
    Application* Application::m_instance = nullptr;

    Application::~Application() {    
        m_layers.destroy();   
        m_window->quit();
        delete m_window;
        FRACTAL_LOG("Destroying Application"); 
    }

    void Application::initialize(const char* name, uint32_t width, uint32_t height, int flags) {
        m_instance = this;
        m_properties = WindowProperties(name, width, height, flags);
        m_window = Window::create_glfw_window(m_properties, BIND_EVENT(on_event));
        FRACTAL_LOG("Initalized application '%s' with size %d by %d", name, width, height);

        RendererCommands::initialize();
        m_imgui_layer = new ImGuiLayer();
        push_layer(m_imgui_layer);
        FRACTAL_LOG("Initialization complete");


        on_create();
    }

    void Application::push_layer(Layer* layer) {
        FRACTAL_LOG("Created new layer '%s'", layer->get_name().c_str());
		m_layers.push_layer(layer);
        layer->on_attach();
    }

    float const Application::get_frame_time() {
        return m_current_frame_time;
    }

    void Application::run() {
        float previous_time = Time::get_time();
        float last_frame_time = 0.0f;
        int interm_fps = 0;

        while (!m_window->destroyed()) {
        	float current_time = Time::get_time(); 
            m_current_frame_time = current_time - last_frame_time;
            last_frame_time = current_time;

            interm_fps++;
			if (current_time - previous_time > 1.0f) {
                m_fps = interm_fps;
                interm_fps = 0;
                previous_time = current_time;
            }

            for (Layer* layer : m_layers)
                layer->on_update(m_current_frame_time);

            m_imgui_layer->begin();

			for (Layer* layer : m_layers)
				layer->update_gui();
            on_gui();

            m_imgui_layer->end();

            m_window->update();
            on_update();
        }
    }

    int const Application::get_fps() {
        return m_fps;
    }

    void Application::on_event(Event& event) {
		EventDispatcher dispatcher(&event);

		on_user_event(event);

		dispatcher.dispatch<QuitEvent>(BIND_EVENT(on_close));
		dispatcher.dispatch<ResizeEvent>(BIND_EVENT(on_resize));
	}

	void Application::on_close(const QuitEvent& event) {
		m_window->destroy();
	}

	void Application::on_resize(const ResizeEvent& event) {
		m_window->properties()->m_width = event.m_width;
		m_window->properties()->m_height = event.m_height;

        RendererCommands::set_viewport(0, 0, event.m_width, event.m_height);
	}
} 