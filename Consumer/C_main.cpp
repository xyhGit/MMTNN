#include <cstdlib>
#include <iostream>
#include <time.h>
#include <unistd.h>

#include "Consumer.h"

int main(int argc, char** argv)
{
	try {
		// The default Face will connect using a Unix socket, or to "localhost".
		Face face;

		// Counter holds data used by the callbacks.
		Consumer consumer;

		char tmp[20]="/vide1/01";
		Name name;
		for (int i = 0; i < 200; i++)
		{
			//Name name("/video/");
			
			tmp[8]=i+'0';
			
			name.set(tmp);
			time_t rawtime;
			time(&rawtime);
			name.appendTimestamp(rawtime);
	
			cout << "Express name " << name.toUri() << endl;
			// Use bind to pass the counter object to the callbacks.
			face.expressInterest(name, bind(&Consumer::onData, &consumer, _1, _2), bind(&Consumer::onTimeout, &consumer, _1));

			// The main event loop.
			while (consumer.callbackCount_ < 1) {
				face.processEvents();
				// We need to sleep for a few milliseconds so we don't use 100% of the CPU.
				usleep(10000);
			}
			consumer.callbackCount_ = 0;
		}
	}
	catch (std::exception& e) {
		cout << "exception: " << e.what() << endl;
	}
	return 0;
}
