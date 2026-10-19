/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <stdint.h>

namespace Systems {

	class CrashLog {

	public:
		// Installs a vectored exception handler that dumps the faulting address, the
		// registers and the plausible return addresses found on the stack into
		// ImprovedCameraSE.crash.log, next to the plugin's own log.
		static void Install();

	private:
		CrashLog() = delete;
	};

}
