#ifndef ENTRY_POINT_H
#define ENTRY_POINT_H

#include "application.h"
#include "log.h"

extern Fractal::Application* Fractal::create_application();

int main(int argc, char *argv[]) {    
    Fractal::initialize_logging_system();
    Fractal::Logs::intialize_loggers();
    
    Fractal::Application* application = Fractal::create_application();

    application->run();

    delete application;
    FRACTAL_LOG("Application closed");
}

#endif // !ENTRY_POINT_H