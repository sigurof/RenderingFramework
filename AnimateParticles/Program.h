//#ifndef PROGRAM_H
//#define PROGRAM_H
//#pragma once
//
//using namespace ML;
//
//class Program
//{
//public:
//	void open()
//	{
//
//
//	}
//
//	void run() {
//		try
//		{
//			double nextTime = times[1]; unsigned int i_time = 1;
//			t = 0;
//			while (ge.windowIsOpen())
//			{
//				timer.start();
//				ge.drawScene();
//				ge.pollUserInput();
//				if ((t += timer.elapsedS()) >= nextTime)
//				{
//					updateEntityPositions(i_time);
//					timer.reset();
//					if (i_time < nFrames - 1)
//					{
//						nextTime = times[++i_time];
//					}
//				}
//			}
//			ge.cleanUp();
//		}
//		catch (const std::exception& err)
//		{
//			std::cout << "In Animator::play():\n";
//			throw err;
//		}
//	}
//
//
//private:
//
//};
//
//#endif
