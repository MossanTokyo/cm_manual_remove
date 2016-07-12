#pragma once

#include<iostream>
#include<vector>
#include<boost/filesystem/path.hpp>
#include<boost/filesystem.hpp>
#include <boost/foreach.hpp>

namespace fs = boost::filesystem;

// this is terrible programming habit, but anyway it is okay for this small program!
// please modify these extensions for your purp
std::vector<std::string> target_exts = { ".m2ts", ".mp4", ".wav" };

std::string output_ext = ".mp4";


// obtain program path
fs::path program_dir(char * arg_path[])
{
	fs::path return_val= arg_path[0];

	std::cout << "starting manual commercial cut in " << return_val.parent_path().string() << std::endl;

	return return_val.parent_path();
}

std::vector<fs::path> find_files(const boost::filesystem::path & program_dir)
{
	namespace fs = boost::filesystem;

	std::vector<fs::path> return_val;

	BOOST_FOREACH(const fs::path& p, std::make_pair(fs::directory_iterator(program_dir), fs::directory_iterator())) 
	{
		if (!fs::is_directory(p))
		{
			for (auto & ext : target_exts)
			{
				if (p.extension().string() == ext)
				{
					return_val.push_back(p);

					std::cout << p.string() << std::endl;
				}
			}
		}
	}

	return return_val;
}

fs::path create_dir(const fs::path & program_dir)
{
	fs::path dir_path = program_dir / "done";

	if (!fs::exists(dir_path))
	{
		// create new directory
		fs::create_directory(dir_path);
	}

	return dir_path;
}

std::vector<fs::path> get_unprocessed_file(const fs::path & program_dir)
{
	std::vector<fs::path> return_val;

	std::vector<fs::path> unprocessed = find_files(program_dir);

	fs::path processed_path = create_dir(program_dir);

	if (unprocessed.size() == 0)
	{
		std::cout << "no video file was found in a program directory.\n";
		exit(0);
	}

	fs::path processed_dir = create_dir(program_dir);

	std::vector<fs::path> processed_files = find_files(processed_dir);

	std::cout << "these fiels will be processed.\n";

	for (auto & candidate : unprocessed)
	{
		bool check = true;
		for (auto & done : processed_files)
		{
			if (done.stem().string() == candidate.stem().string())
			{
				check = false;
			}
		}

		if (check)
			return_val.push_back(candidate);

		std::cout << candidate.string() << std::endl;
	}

	return return_val;
}