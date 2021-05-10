#include <iostream>
#include <string>
#include <assert.h>
#include <leveldb/db.h>
#include <vector>
#include <fstream>
#include <random>
#include <chrono>

using namespace std;
extern std::chrono::nanoseconds n_yi;

int max_entries = 1000000;
int* arr = new int [max_entries];

string db_name;

string tests[] = {
	"-b=fillseq",
	"-b=fillrand",
	"-b=readseq",
	"-b=readrand",
	"-b=update",
	"-b=purelyrandom"
};

void print_menu() {
	cout << "Usage: ./main -b=$TEST $OPTION\n";
	cout << "	Available $TEST:" << endl;
	cout << "	fillseq		fill empty database with 1M entries in sequential order\n";
	cout << "       fillrand	fill empty database with 1M entries in random order\n";
	cout << "       readseq		read all 1M entries in sequential order. MUST execute fillseq or fillrand first\n";
	cout << "       readrand	read all 1M entries in random order.  MUST execute fillseq or fillrand first\n";
	cout << "       update		update all 1M entries randomly (not all entries are updated).  MUST execute fillseq or fillrand first\n";
	cout << "       purelyrandom	randomly do R&W operation to random entry. NEED to add $OPTION field.\n";
	cout << "		Available $OPTION = # of tests, write frequency (1-100)\n";
}

void test_fillseq(leveldb::DB *db) {
	leveldb::Status status;
	string key;
	string token;
	for (int i = 0;  i < max_entries; i++) {
		key = to_string(i);
		token = to_string((int) rand() % max_entries);
		status = db->Put(leveldb::WriteOptions(), key, token);
		if (!status.ok()) {
			cerr << "[ERROR]" << status.ToString() << endl;
			return;
		}
	}
}

void test_fillrand(leveldb::DB *db) {
	leveldb::Status status;
	string key;
	string token;
	for (int i = 0; i < max_entries; i++) {
		key = to_string(arr[i]);
		token = to_string((int) rand() % max_entries);
		status = db->Put(leveldb::WriteOptions(), key, token);
		if (!status.ok()) {
			cerr << "[ERROR]" << status.ToString() << endl;
			return;
		}
	}
}

void test_readseq(leveldb::DB *db) {
	leveldb::Status status;
	string key;
	string token;
	for (int i = 0;  i < max_entries; i++) {
		key = to_string(i);
		status = db->Get(leveldb::ReadOptions(), key, &token);
		if (!status.ok()) {
			cerr << "[ERROR]" << status.ToString() << endl;
			return;
		}
	}
}

void test_readrand(leveldb::DB *db) {
	leveldb::Status status;
	string key;
	string token;
	for (int i = 0;  i < max_entries; i++) {
		key = to_string(arr[i]);
		status = db->Get(leveldb::ReadOptions(), key, &token);
		if (!status.ok()) {
			cerr << "[ERROR]" << status.ToString() << endl;
			return;
		}
	}
}

void test_update(leveldb::DB *db) {
	leveldb::Status status;
	string key;
	string token;
	for (int i = 0; i < max_entries; i++) {
		key = to_string((int) rand() % max_entries);
		token = to_string((int) rand());
		status = db->Put(leveldb::WriteOptions(), key, token);
		if (!status.ok()) {
			cerr << "[ERROR]" << status.ToString() << endl;
			return;
		}
	}
}

void test_purelyrandom(leveldb::DB *db, int max_tests, int write_frequency, int key_range) {
	std::default_random_engine generator(time(NULL));
	std::uniform_int_distribution<int> d(0,100);
	std::uniform_int_distribution<long int> d1(0, key_range);

	std::string token;
	std::string key;

	leveldb::Status status;

	//	std::vector<string> s;
	for (long int i = 0; i < max_tests; i++) {
		key = to_string(d1(generator));

		int operation = (int) rand() % 100;

		if (operation <= write_frequency) {
			token = to_string(d1(generator));
			status = db->Put(leveldb::WriteOptions(), key, token);
			if (!status.ok()) {
				cerr << "[ERROR]" << status.ToString() << endl;
				return;
			}
		} else {
			status = db->Get(leveldb::ReadOptions(), key, &token);
		}

	}
}

void my_swap(int* a, int from, int to) {
	int tmp = a[from];
	a[from] = a[to];
	a[to] = tmp;
}

void fill_random_array() {
	for (int i = 0; i < max_entries; i++) {
		arr[i] = i;
	}

	for (int i = 0; i < max_entries; i++) {
		int from = (int) rand() % max_entries;
		int to = (int) rand() % max_entries;
		my_swap(arr, from, to);
	}
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	string this_test;

	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;

	std::chrono::nanoseconds n(0);
	std::chrono::time_point<std::chrono::system_clock> start, end;

	if (argc > 2) {
		db_name = argv[2];
		this_test = argv[1];
		if (this_test == tests[0]) {
			//			system(string("rm -rf ") + db_name + string("/"));
			start = std::chrono::system_clock::now();
			leveldb::Status status = leveldb::DB::Open(options, db_name, &db);
			if (!status.ok()) {
				delete arr;
				return 1;
			}
			test_fillseq(db);
		} else if (this_test == tests[1]) {
			//			system(string("rm -rf ") + db_name + string("/"));
			fill_random_array();
			start = std::chrono::system_clock::now();
			leveldb::Status status = leveldb::DB::Open(options, db_name, &db);
			if (!status.ok()) {
				delete arr;
				return 1;
			}
			test_fillrand(db);
		} else if (this_test == tests[2]) {
			start = std::chrono::system_clock::now();
			leveldb::Status status = leveldb::DB::Open(options, db_name, &db);
			if (!status.ok()) {
				delete arr;
				return 1;
			}
			test_readseq(db);
		} else if (this_test == tests[3]) {
			fill_random_array();
			start = std::chrono::system_clock::now();
			leveldb::Status status = leveldb::DB::Open(options, db_name, &db);
			if (!status.ok()) {
				delete arr;
				return 1;
			}
			test_readrand(db);
		} else if (this_test == tests[4]) {
//			fill_random_array();
			start = std::chrono::system_clock::now();
			leveldb::Status status = leveldb::DB::Open(options, db_name, &db);
			if (!status.ok()) {
				delete arr;
				return 1;
			}
			test_update(db);
		} else if (this_test == tests[5]) {
			if (argc != 6) {
				cout << "ERROR, enter # of tests, write frequency (1-100)\n";

			}

			long int max_tests = atoi(argv[3]);
			int write_frequency = atoi(argv[4]);
			int key_range = atoi(argv[5]);
			leveldb::Status status = leveldb::DB::Open(options, db_name, &db);
			start = std::chrono::system_clock::now();
			test_purelyrandom(db, max_tests, write_frequency, key_range);
		} else {
			print_menu();
			delete arr;
			return 1;
		}
	} else {
		print_menu();
		delete arr;
		return 1;
	}
	end = std::chrono::system_clock::now();

	auto duration = end - start;
	n += std::chrono::duration_cast<std::chrono::nanoseconds>(duration);
	cout << "Total program time is " << n.count() << endl;
	cout << "Total compaction time is xxxxxx " << n_yi.count() << endl;

	cout << "Compaction fraction is " << (double) n_yi.count() / (double) n.count() * 100 << "%" << endl;

	delete db;
	delete arr;

	return 0;
}

