#pragma once
#include "Engine.h"

namespace Cae {
	class App {
	public:
		App();
		virtual ~App();

		void Run();
	private:
		Engine* engine;
	};
}