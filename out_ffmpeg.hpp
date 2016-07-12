#pragma once

#include"display_video.hpp"
#include"list_files.hpp"
#include<boost/filesystem/fstream.hpp>

void trim(fs::ofstream & out, fs::path & file, std::vector<std::pair<int, int>> & contents, fs::path ffmpeg_path, std::string ffmpeg_options)
{
	int i = 0;

	int gap = 3;

	// trim command
	for (auto & elem : contents)
	{
		out << ffmpeg_path << " -y -ss " << (((elem.first - gap) < 0) ? 0 : elem.first - gap) << " -i \"" << file.string() << "\" -t " << elem.second - elem.first + gap << "  -vcodec copy -acodec copy tmp_" + std::to_string(i++) + ".m2ts\n";
	}

	// concastrate command

	// ffmpeg path
	out << ffmpeg_path;

	for (auto j = 0; j < i; ++j)
	{
		out << " -i tmp_" << std::to_string(j) << ".m2ts";
	}

	// ffmpeg commands
	out << " -filter_complex \"yadif=0:-1,concat=n=" << std::to_string(i) << ":v=1:a=1\" " << ffmpeg_options << " \"done\\" << file.stem().string() << ".mp4\"\n";
}