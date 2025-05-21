#include "fcplay.h"

int main(int argc, char *argv[])
{
	sys::config::args(argc, argv);

	cout << ("--------------------------------------------------") << endl;
	cout << ("AMIGA Future Composer v1.x SMOD/FC14 music player" ) << endl;
	cout << ("--------------------------------------------------") << endl;

	for(int i = optind; i < argc; i++)
		player::queue(argv[i]);

	if(player::playlist.size() > 0)
	{
		sys::audio::init();
		player::init();

		sys::input::init();
		for(size_t i = 0; i < player::playlist.size(); i++)
			player::play(i);

		player::halt();
		sys::audio::halt();
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}
