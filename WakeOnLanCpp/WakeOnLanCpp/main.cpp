#include "wol.h"



int main() {

	try {

		WOL wol;
		wol.setup();
		wol.sendMagicPackets();

	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
					
	system("pause");
	return EXIT_SUCCESS;
}