#include <beingEngine/core/core.hpp>
#include <virtualGame/application/application.hpp>

#include <cstdio>

int main() {
	printf("\n\n> My lawyers advised against making a whole engine for a game jam.\n");

	try {
		Application application;
		application.start();
	} catch (const std::exception& exception) {
		printf("%s\n", exception.what());
	}
}