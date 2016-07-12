#pragma once

#include<boost/filesystem/path.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<map>
#include <cstdlib>
#include<algorithm>
#include<limits>

int target_width = 500;

void clear_screen()
{
#ifdef _WIN32
	std::system("cls");
#else
	// Assume POSIX
	std::system("clear");
#endif
}

class cm_check
{
private:
	cv::VideoCapture video;

	cv::Mat image;

	double fps;

	int width;

	double ratio;

	bool status;

	double previous_t;
	double current_t;

	struct pair_comp
	{
		bool operator()(const std::pair<int, int> & a, const std::pair<int, int> & b)
		{
			return a.first < b.first;
		}
	};

	std::vector<std::pair<int, int>> main_contents;

	int start;
	int end;

	const int KEYCODE_ONE;
	const int KEYCODE_TWO;
	const int KEYCODE_THREE;
	const int KEYCODE_FOUR;
	const int KEYCODE_FIVE;
	const int KEYCODE_SIX;
	const int KEYCODE_SEVEN;
	const int KEYCODE_EIGHT;
	const int KEYCODE_NINE;
	const int KEYCODE_ESC;
	const int KEYCODE_PLUS;
	const int KEYCODE_MINUS;
	const int KEYCODE_a;
	const int KEYCODE_s;
	const int KEYCODE_d;

	int key;

	std::string name;

	double frame_count;

	bool success;
public:

	std::vector<std::pair<int, int>> get()
	{
		return main_contents;
	}

	bool validate(std::pair<int, int> input)
	{
		if (input.second <= input.first)
		{
			return false;
		}

		bool first = true;

		if (main_contents.size() == 0)
		{
			return true;
		}

		if (main_contents.back().second < input.first)
			return true;

		if (main_contents.front().first > input.second)
			return true;

		for (int i = 0; i < main_contents.size(); ++i)
		{
			if (main_contents[i].first == input.second || main_contents[i].second == input.first)
			{
				false;
			}

			if (main_contents[i].first > input.first)
			{
				if (input.second < main_contents[i].second)
					return true;

				return false;
			}
		}
	}

	cm_check() = delete;

	cm_check(const boost::filesystem::path &file) : video(file.string()), name(file.stem().string()),
	start(-1), end(-1),
	current_t(0), previous_t(0),
	KEYCODE_ONE(49),
	KEYCODE_TWO(50),
	KEYCODE_THREE(51),
	KEYCODE_FOUR(52),
	KEYCODE_FIVE(53),
	KEYCODE_SIX(54),
	KEYCODE_SEVEN(55),
	KEYCODE_EIGHT(56),
	KEYCODE_NINE(57),
	KEYCODE_ESC(27),
	KEYCODE_PLUS(43),
	KEYCODE_MINUS(45),
	KEYCODE_a(97),
	KEYCODE_d(100),
	KEYCODE_s(115)
	{
		// extract filename
		if (video.isOpened())
		{
			success = true;

			status = false;

			// frames per second
			fps = video.get(CV_CAP_PROP_FPS);
			// width
			width = video.get(CV_CAP_PROP_FRAME_WIDTH);

			if (width == 1440)// HD
			{
				ratio = 400.0 / 1440.0;
			}
			else if (width == 1920) // full HD
			{
				ratio = 400.0 / 1920.0;
			}
			else if (width == 4096 || width == 3840) // 4K
			{
				ratio = 400.0 / 3840;
			}
			else
			{
				ratio = 1.0;
			}

			cv::namedWindow("window", CV_WINDOW_AUTOSIZE);

			// video.get(CV_CAP_PROP_FRAME_COUNT) does not return correct answer
			frame_count = video.get(CV_CAP_PROP_FRAME_COUNT);

			// wow, opencv cannt get correct frame_count, so find correct one

			bool check = true;

			int true_count = frame_count;

			while (check)
			{
				video.set(CV_CAP_PROP_POS_FRAMES, true_count);
				if (video.grab())
				{
					check = false;
				}

				//////////////////////////////////
				// please modify me for better accuracy
				//////////////////////////////////
				true_count -= 10;
			}

			frame_count = true_count;

			video.set(CV_CAP_PROP_POS_FRAMES, 1);

			video >> image;

			cv::resize(image, image, cv::Size(), ratio, ratio);

			cv::imshow("window", image);

			show();

			key = cv::waitKey(0);
		}
		else
		{
			success = false;
		}
	}

	bool ok()
	{
		return success;
	}

	void skip(const int & second)
	{
		previous_t = current_t;

		if ((current_t + second) * fps > frame_count)
		{
			current_t = std::floor(frame_count / fps);
		}
		else
		{
			current_t += second;
		}

		video.set(CV_CAP_PROP_POS_FRAMES, current_t * fps);
	}

	void back(const int & second)
	{
		previous_t = current_t;

			if (current_t - second > 0)
			{
				if (current_t == std::numeric_limits<int>::max())
				{
					current_t = frame_count / fps - second;
				}
				else
				{
					current_t -= second;
				}
			}
			else
			{
				current_t = 0;
			}

			video.set(CV_CAP_PROP_POS_FRAMES, current_t * fps);


	}

	void input()
	{
		video >> image;
		
		if (current_t * fps > frame_count)
		{
			video.set(CV_CAP_PROP_POS_FRAMES, frame_count);
		}
		else
		{
			video.set(CV_CAP_PROP_POS_FRAMES, current_t * fps);
		}

		cv::resize(image, image, cv::Size(), ratio, ratio);

		cv::imshow("window", image);

		key = cv::waitKey(0);
	}

	bool command()
	{
		std::cout << "input: ";

		if (KEYCODE_ONE == key)
		{
			std::cout << "1\n";
			skip(5);
			return true;
		}
		else if (KEYCODE_TWO == key)
		{
			std::cout << "2\n";
			skip(10);
			return true;
		}
		else if(KEYCODE_THREE == key)
		{
			std::cout << "3\n";
			skip(30);
			return true;
		}
		else if (KEYCODE_FOUR == key)
		{
			std::cout << "4\n";
			back(5);
			return true;
		}
		else if (KEYCODE_SIX == key)
		{
			std::cout << "6\n";
			skip(60);
			return true;
		}
		else if (KEYCODE_SEVEN == key)
		{
			std::cout << "7\n";
			back(10);
			return true;
		}
		else if (KEYCODE_EIGHT == key)
		{
			std::cout << "8\n";
			back(30);
			return true;
		}
		else if (KEYCODE_NINE == key)
		{
			std::cout << "9\n";
			skip(300);
			return true;
		}
		else if (KEYCODE_ESC == key)
		{
			std::cout << "ESC\n";
			return false;
		}
		else if (KEYCODE_PLUS == key)
		{
			std::cout << "+\n";
			skip(1);
			return true;
		}
		else if (KEYCODE_MINUS == key)
		{
			std::cout << "-\n";
			back(1);
			return true;
		}
		else if (KEYCODE_a == key)
		{
			show_main_contents();
			return true;
		}
		else if (KEYCODE_d == key)
		{
			std::cout << "d\n";

			status = false;
			start = -1;
			end = -1;

			main_contents.pop_back();

		}
		else if (KEYCODE_FIVE == key)
		{
			std::cout << "5\n";

			if (start == -1)
			{
				start = current_t;

				status = true;
			}
			else if (end == -1)
			{
				status = false;

				end = current_t;

				if (validate(std::make_pair(start, end)))
				{
					main_contents.push_back(std::make_pair(start, end));

					start = -1;
					end = -1;
				}
				else
				{
					std::cout << "inproper inputs for start/end. inputs are reset.\n";
					start = -1;
					end = -1;
				}
			}

			return true;
		}
		else
		{
			return true;
		}
	}

	void show_main_contents()
	{
		std::cout << "\n\n main contents info.\n";

		int i = 0;

		for (; i < main_contents.size(); ++i)
		{
			std::cout << "#" << i << ": start = " << main_contents[i].first << ", end = " << main_contents[i].second << std::endl;
		}

		if (start != -1)
		{
			std::cout << " current input: start = " << start << ", end = NA" << std::endl;
		}
		else
		{
			if (main_contents.size() == 0)
			{
				std::cout << "no input avaialble.\n";
			}
		}
			
	}

	void show()
	{
		clear_screen();

		std::cout << "processing " << name << std::endl;
		std::cout << "FPS: " << fps << std::endl;
		std::cout << "total frames:" << frame_count << std::endl;
		std::cout << "current_t: " << current_t << std::endl;
		std::cout << "previous_t: " << previous_t << std::endl;
		std::cout << "current status: ";

		if (status)
		{
			std::cout << "set the end of commercial.\n";
		}
		else
		{
			std::cout << "set the start of commercial.\n";
		}

		std::cout << "5: set the current time step as start/end of commercial\n";
		std::cout << "9: skip 5 minutes\n";
		std::cout << "6: skip 1 minute\n";
		std::cout << "3: skip 30 seconds\n";
		std::cout << "2: skip 10 seconds\n";
		std::cout << "1: skip 5 seconds\n";
		std::cout << "+: skip 1 second\n";
		std::cout << "-: back 1 second\n";
		std::cout << "4: back 5 second\n";
		std::cout << "7: back 10 seconds\n";
		std::cout << "8: back 30 seconds\n";
		
		std::cout << "ESC: finish this video\n";
		std::cout << "d: delete last main content.\n";
		std::cout << "a: show current main contents info.\n";

		if (KEYCODE_a == key)
		{
			show_main_contents();
		}
	}
};
