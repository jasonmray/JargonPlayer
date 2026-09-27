#include "Jargon/StreamUtilities.h"

#include <iostream>
#include <io.h>


namespace Jargon{
namespace StreamUtilities {

	bool isStdinInteractive() {
		if (_isatty(_fileno(stdin))) {
			return true;
		}
		return false;
	}

	bool isStdinPiped() {
		return !isStdinInteractive();
	}

}
}
