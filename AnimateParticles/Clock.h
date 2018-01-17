#ifndef CLOCK_H
#define CLOCK_H
#pragma once


namespace ML {


	class Clock
	{
	public:
		Clock() : begin(std::chrono::system_clock::now()), end(std::chrono::system_clock::now()) {}

		void start() {
			begin = std::chrono::system_clock::now();
		}

		int elapsedMs() const
		{
			return (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - begin).count();
		}

		float elapsedS() const 
		{
			return (float)elapsedMs() / 1000.f;
		}

	private:

		std::chrono::system_clock::time_point begin, end;
	};

}













#endif // !CLOCK_H