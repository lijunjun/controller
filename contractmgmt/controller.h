
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <string>

#define CONSOLE_WAIT(iEndString) \
	std::string lEnd; \
	while (true) \
	{ \
		std::cin >> lEnd; \
		if (lEnd == iEndString) break; \
	}



#endif
