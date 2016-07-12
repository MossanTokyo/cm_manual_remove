#include<vector>
#include<string>

#include"list_files.hpp"
#include"display_video.hpp"
#include"out_ffmpeg.hpp"

void load_settings(const fs::path & dir, fs::path & ffmpeg_path, std::string & ffmpeg_options)
{
	fs::path settings = dir / "settings.txt";

	fs::ifstream file(settings);

	std::string tmp;

	std::getline(file, tmp);

	ffmpeg_path = tmp;

	std::getline(file, ffmpeg_options);
}

int main(int argc, char *argv[])
{
	

	auto dir = program_dir(argv);

	// load ffmpeg...
	fs::path ffmpeg_path;
	// load ffmpeg options
	std::string ffmpeg_options;

	// load settings.
	load_settings(dir, ffmpeg_path, ffmpeg_options);

	std::cout << "using ffmpeg at " << ffmpeg_path.string() << std::endl;

	auto unprocessed_files = get_unprocessed_file(dir);

	fs::ofstream out("out.bat");

	for (auto & video : unprocessed_files)
	{
		std::cout << video << std::endl;

		cm_check manual(video);

		if (manual.ok())
		{
			while (manual.command())
			{
				manual.show();
				manual.show_main_contents();
				manual.input();
			}
		}

		clear_screen();

		std::cout << "selected contents for " << video << std::endl;
		manual.show_main_contents();

		trim(out, video, manual.get(), ffmpeg_path, ffmpeg_options);
	}

	return 0;
}