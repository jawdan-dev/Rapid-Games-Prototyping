#include <beingEngine/application/application.hpp>
#include <beingEngine/core/core.hpp>

#include <cstdio>

int main() {
    printf("\n\n> My lawyers advised against this.\n");

    try {
        Application application;
        application.start();
    } catch (const std::exception &exception) {
        printf("%s\n", exception.what());
    }
}