/**
gcc -o mdclient mdclient.c -I/usr/local/czmq/include -I/usr/local/zeromq/include 
-L/usr/local/zeromq/lib -lzmq -L/usr/local/czmq/lib -lczmq
*/

#include "mdcliapi.c"

int main (int argc, char *argv[])
{
	int verbose = (argc > 1 && streq(argv[1], "-v"));
	mdcli_t *session = mdcli_new("tcp://127.0.0.1:5555", verbose);

	int count;
	for (count = 0; count < 100000; count++){
		zmsg_t *request = zmsg_new();
		zmsg_pushstr(request, "Hello world");
		zmsg_t *reply = mdcli_send(session, "echo", &request);
		if (reply)
			zmsg_destroy(&reply);
		else 
			break;
	}
	printf("%d requests/replies processed\n", count);
	mdcli_destroy(&session);
	return 0;
}