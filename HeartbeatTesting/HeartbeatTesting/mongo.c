#include "../packages/mongo/src/libmongoc/src/mongoc/mongoc.h"


int handle_mongo(int argc, const char argv[]) {

	const char *mongo_uri "mongodb://localhost:27017"
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
		mongo_uri = argv[1];
	}

	uri = mongo_uri_new_with_error(mongo_uri, &error);

	if (!uri) {

	}
}