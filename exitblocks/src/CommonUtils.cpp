/*#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
 */
#include <string.h>

#include "CommonUtils.h"

#include <iostream>

char
CommonUtils::askQuestion(const char *q, const char *opts)
{
	char yn;
	do {
		std::cout  << q;
		std::cin >> yn;
		if (!std::cin.fail()) {
			bool f = false;
			for (unsigned i = 0; i < strlen(opts); i++) {
				if (opts[i] == yn) {
					return yn;
				}
			}
		}
	} while (true);
	return (char)0;
}

