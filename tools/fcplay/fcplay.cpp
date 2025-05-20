#include "fcplay.h"

int main(int argc, char *argv[])
{
	sys::config::args(argc, argv);

	cout << ("--------------------------------------------------") << endl;
	cout << ("Future Composer 1.0 - 1.4 (AMIGA) Music Player"    ) << endl;
	cout << ("--------------------------------------------------") << endl;

	for(int i = optind; i < argc; i++)
		player::queue(argv[i]);
	player::init();

	if(player::playlist.size() > 0)
	{
		sys::audio::init();

		sys::input::init();
		for(size_t i = 0; i < player::playlist.size(); i++)
			player::play(i);

		sys::audio::halt();
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}
