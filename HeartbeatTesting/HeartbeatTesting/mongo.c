#include "../packages/mongo/src/libmongoc/src/mongoc/mongoc.h"

#define MONGO_URI "mongodb://localhost:27017"

int handle_mongo(int argc, const char argv[]) {

	mongo_uri_t *uri;
	mongo_client_t *client;
	mongo_database_t *database;
	mongoc_collection_t *collection;
	bson_t *command, reply, *insert;
	bson_error_t error;
	char *str;
	bool retval;

	mongo_init();

	if (argc > 1) {

	}
}