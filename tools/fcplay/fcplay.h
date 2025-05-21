#pragma once

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdint>
#include <cstddef>
#include <csignal>
#include <cstring>
#include <filesystem>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>

#include <getopt.h>
#include <unistd.h>

#include <alsa/asoundlib.h>

#include "fc14audiodecoder.h"

using namespace std;
using namespace std::filesystem;
 
namespace sys
{
	bool done = false;

void print_usage(const char* program_file)
{
	cerr << "Usage: " << program_file << " [OPTION]... [FILE]..." << std::endl;
	cerr << "Play Future Composer Tracker .fc, .fc13, .fc14, .fc3, .fc4 audio FILEs (the current directory by default)." << std::endl << std::endl;

	cerr << "Mandatory arguments to long options are mandatory for short options too." << std::endl;
	cerr << "  -r, --recursive             recursively traverse directories" << std::endl;
	cerr << "  -b TIME_MS, --beg=TIME_MS   beg time of the song in milliseconds" << std::endl;
	cerr << "  -e TIME_MS, --end=TIME_MS   end time of the song in milliseconds" << std::endl;
	cerr << std::endl;
}

namespace config
{
	bool recursive = false;
	bool ttyout    = true;
	constexpr static const uint32_t FC_silent_data_len = 8 + 1;	// see FC.cpp
	namespace step
	{
		uint32_t beg = 0;
		uint32_t end = 0;
		uint32_t len = 0;
		uint32_t cur = 0; 
	};
	namespace audio
	{
		constexpr static const unsigned int DEFAULT_PCM_FREQ    = 44100;
		constexpr static const unsigned int DEFAULT_BITS        = 16;
		constexpr static const unsigned int DEFAULT_CHANNELS    = 2;
		constexpr static const unsigned int DEFAULT_BUFFER_SIZE = 4096;
		unsigned int pcm_freq             = DEFAULT_PCM_FREQ;
		unsigned int bits                 = DEFAULT_BITS;
		unsigned int channels             = DEFAULT_CHANNELS;
		unsigned int buffer_size          = DEFAULT_BUFFER_SIZE;
		static const unsigned int silent_output_sample()
		{
			switch(bits)
			{
				case 16: return 0x0000;
				case 8 : return 0x80;
				default: return -1;
			}
		}
	};

	void args(int argc, char* argv[])
	{
		if(argc < 1)
		{
			print_usage(argv[0]);
			exit(EXIT_FAILURE);
		}

		int opt;
		int option_index;

		static struct option long_options[] =
		{
			{ "recursive",       no_argument,    0, 'r' },
			{ "begin"    , required_argument,    0, 'b' },
			{ "end"      , required_argument,    0, 'e' },
			{ "file"     , required_argument,    0,  0  }, 
			{ NULL       ,                 0, NULL,  0  }
		};
		while((opt = getopt_long(argc, argv, "rb:e:", long_options, &option_index)) != -1)
		{
			switch(opt)
			{
				case 'r':
					recursive = true;
					break;
				case 'b':
					if(optarg)
						step::beg = atoi(optarg);
					break;
				case 'e':
					if(optarg)
						step::end = atoi(optarg);
					break;
				default:
					print_usage(argv[0]);
					exit(EXIT_FAILURE);
					break;	
			}
		}

		if(optind == argc) { sys::print_usage(argv[0]); exit(EXIT_FAILURE); }
	}
};
namespace input
{
	struct old
	{
		void (*sighup)  (int);
		void (*sigint)  (int);
		void (*sigquit) (int);
		void (*sigterm) (int);
	};

	void handler(int signum)
	{
		switch (signum)
		{
			case SIGHUP:  done = true; break;
			case SIGINT:  done = true; break;
			case SIGQUIT: done = true; break;
			case SIGTERM: done = true; break;
			default:                   break;
		}
	}
	bool init()
	{
		return ((signal(SIGHUP , &sys::input::handler) == SIG_ERR)
		     || (signal(SIGINT , &sys::input::handler) == SIG_ERR)
		     || (signal(SIGQUIT, &sys::input::handler) == SIG_ERR)
	             || (signal(SIGTERM, &sys::input::handler) == SIG_ERR)) ? false : true;
	}
};

namespace audio
{
	using handle = snd_pcm_t;
	using params = snd_pcm_hw_params_t;
	sys::audio::params *hw_params;
	sys::audio::handle *playback_handle;
	std::vector<uint8_t> sample_buf;

	bool init()
	{
		bool ret = !(snd_pcm_open(&playback_handle, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0 ||
		snd_pcm_hw_params_malloc(&hw_params) < 0 ||
		snd_pcm_hw_params_any(playback_handle, hw_params) < 0 ||
		snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED) < 0 ||
		snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_S16_LE) < 0 ||
		snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, (unsigned int *) &sys::config::audio::pcm_freq, 0) < 0 ||
		snd_pcm_hw_params_set_channels(playback_handle, hw_params, 2) < 0 ||
		snd_pcm_hw_params(playback_handle, hw_params) < 0 ||
		snd_pcm_prepare(playback_handle) < 0);
		
		snd_pcm_hw_params_free(hw_params);
		sample_buf.resize(sys::config::audio::pcm_freq);
		return ret;
	}
	bool halt()
	{
		return snd_pcm_close(playback_handle) < 0;
	}
};

};

namespace player
{
	void* decoder = fc14dec_new();
	std::vector<path> playlist;
	int max_file_size_len   = 0;
	int max_song_length_len = 0;
	size_t memory_usage = 0;
	struct time
	{
		size_t size;
		std::string name;
		std::string author;
		std::string title;
		unsigned long int beg;
		unsigned long int cur;
		unsigned long int end;
		unsigned long int len;
		std::vector<uint8_t> buf;
		void print()
		{
			printf("\r%0*zu %0*zu/%0*zu %s %s ", max_file_size_len, buf.size(), max_song_length_len, cur, max_song_length_len, end, buf.data(), name.c_str());
		}
		bool song_end()
		{
			return cur >= end || fc14dec_song_end(decoder) == 0;
		}
		bool update(unsigned long int ms)
		{
			cur += ms;
			return true;
		}
	};
	std::vector<time> timelist;
	inline int format(const path& src)
	{
		char buf[8] = "";
		FILE* fp = fopen(src.c_str(), "rb");
		if(fread(buf, 8, 1, fp) == 1)
		fclose(fp);
		return fc14dec_detect(decoder, buf, 8);
	}
	bool init()
	{
		bool first = true;
		max_file_size_len = 0;
		max_song_length_len = 0;
		int len = 0;
		int err = 0;
		char str[256];

		for(size_t i = 0; i < playlist.size(); i++)
		{
			if(!exists(playlist[i])) continue;

			size_t size = file_size(playlist[i]);
			if(size == 0) continue;

			timelist[i].buf.resize(size);

			FILE* fp = fopen(playlist[i].c_str(), "rb");
			if(!fp) return false;

			err = fread(timelist[i].buf.data(), timelist[i].buf.size(), 1, fp);
			fclose(fp); if(err != 1) continue;

			if (fc14dec_init(decoder, timelist[i].buf.data(), timelist[i].buf.size()) == 1)
			{
				cerr << "File format not recognized." << endl;
				return false;
			}

			timelist[i].name = playlist[i].filename().replace_extension("");
			/* determine size string dimensions */
			snprintf(str, 255, "%zu", timelist[i].buf.size());
			len = strlen(str);
			if(len > max_file_size_len)
				max_file_size_len = len;
			timelist[i].len = fc14dec_duration(decoder);
			snprintf(str, 255, "%zu", timelist[i].len);
			len = strlen(str);
			if(len > max_song_length_len)
				max_song_length_len = len;

			timelist[i].cur = timelist[i].beg;
			if(timelist[i].end == 0)
				timelist[i].end = timelist[i].len;
			fc14dec_restart(decoder);
			memory_usage += timelist[i].buf.size();
		}
		if(first)
		{
			printf("%zu %0*u/%0*u fmt  name\n",player::memory_usage, max_song_length_len, sys::config::step::beg, max_song_length_len, sys::config::step::end);
			cout << ("--------------------------------------------------") << endl;
			first = false;
		}
		return true;
	}
	bool queue(path src)
	{
		src = !src.is_absolute() ? proximate(src) : src;
		if(exists(src))
		{
			if(!is_directory(src) && !(format(src) > 0))
			{
				playlist.push_back(src);
				timelist.push_back({ .beg = sys::config::step::beg, .cur = 0, .end = sys::config::step::end, .len = 0 }); 
				return true;
			}
			else if(is_directory(src))
			{
				directory_iterator dir_iter(src);
				while(dir_iter != end(dir_iter))
				{
					const directory_entry& dir_entry = *dir_iter++;
					if(!is_directory(dir_entry) || sys::config::recursive)
						queue(dir_entry.path());
				}
			}
		}
		return false;
	}
	bool play(size_t i = 0)
	{
		int err;

		if (fc14dec_init(decoder, timelist[i].buf.data(), timelist[i].buf.size()) == 1)
		{
			cerr << "File format not recognized." << endl;
			return false;
		}

		fc14dec_mixer_init(decoder, sys::config::audio::pcm_freq, sys::config::audio::bits, sys::config::audio::channels, sys::config::audio::silent_output_sample());
		fc14dec_seek(decoder, timelist[i].beg);

		sys::done = false;
		while (!sys::done && !timelist[i].song_end())
		{
			timelist[i].update(fc14dec_buffer_fill(decoder, sys::audio::sample_buf.data(), sys::config::audio::buffer_size >> 2));
			/* display song progress info */
			timelist[i].print();
			/* write to audio interface device */
			if((err = snd_pcm_writei(sys::audio::playback_handle, sys::audio::sample_buf.data(), sys::config::audio::buffer_size >> 4)) != sys::config::audio::buffer_size >> 4)
			{
				fprintf(stderr, "write to audio interface failed (%s)\n", snd_strerror(err));
				exit(1);
			}
		}
		fc14dec_restart(decoder);
		cout << endl;
		return true;
	}
	bool halt()
	{
		fc14dec_delete(decoder);
		return true;
	}
};

