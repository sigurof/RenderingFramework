#ifndef CLOCK_H
#define CLOCK_H
#pragma once

#include <chrono>

namespace ML {


	class Clock
	{
	public:
		Clock() : begin(std::chrono::system_clock::now()), end(std::chrono::system_clock::now()) {}

		void reset() {
			begin = std::chrono::system_clock::now();
			end = std::chrono::system_clock::now();
		}

		void start() {
			begin = std::chrono::system_clock::now();
		}

		int elapsedMs() {
			end = std::chrono::system_clock::now();
			return (int)std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
		
		}

		float elapsedS() {
			return  (float)elapsedMs() / 1000.f;
		}

	private:

		std::chrono::system_clock::time_point begin, end;
	};

}













#endif // !CLOCK_H