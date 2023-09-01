//#ifdef _WIN32
//#define _WIN32_WINNT 0X0A00
//#endif


#include <iostream>
#include <asio.hpp>
#include <chrono>
#include <thread>
#include <mutex>
//#include <future>
#include "./AI.cpp"

AI::node one;
AI::node two;
AI::node three;
AI::node four;
AI::node five;
AI::node six;
AI::node seven;
AI::node eight;

int find_node_index_in_vector(std::vector<AI::node*> vector, AI::node& candidate)
{
	bool stop = false;
	//For debugging
	//if (candidate.name == "four") { stop = true;}

	printf("Finding node in vector : server...");
	int index = 0;
	for (int i = 0; i < vector.size(); i++)
	{
		if (vector.at(i)->name == candidate.name)
		{
			return i;
		}
	}
	return index;
}

std::vector<AI::node> createpath(int& id)
{
	printf("Creating nodes...\n");

	//Name setup
	one.name = "one";
	one.parent = "start";
	one.start = true;
	one.cost_to_reach = 100000;

	two.name = "two";
	three.name = "three";
	four.name = "four";
	five.name = "five";
	six.name = "six";
	seven.name = "seven";
	eight.name = "eight";

	//Setup neighbours

	one.distance_to_neighbours.push_back(3);
	one.distance_to_neighbours.push_back(11);
	one.distance_to_neighbours.push_back(5);
	one.command.push_back("curve_forward_right");
	one.command.push_back("forward");
	one.command.push_back("curve_forward_left");


	two.distance_to_neighbours.push_back(10000000);
	two.distance_to_neighbours.push_back(5);
	two.command.push_back("curve_backward_left");
	two.command.push_back("forward");


	three.distance_to_neighbours.push_back(10000000);
	three.distance_to_neighbours.push_back(11);
	three.command.push_back("curve_backward_right");
	three.command.push_back("forward");


	four.distance_to_neighbours.push_back(10000000);
	four.distance_to_neighbours.push_back(7);
	four.command.push_back("backward");
	four.command.push_back("forward");


	five.distance_to_neighbours.push_back(7);
	five.distance_to_neighbours.push_back(2);
	five.command.push_back("forward");
	five.command.push_back("backward");


	six.distance_to_neighbours.push_back(100000);
	six.distance_to_neighbours.push_back(100000);
	six.distance_to_neighbours.push_back(100000);


	seven.distance_to_neighbours.push_back(5);
	seven.distance_to_neighbours.push_back(7);
	seven.command.push_back("backward");
	seven.command.push_back("curve_forward_right");


	eight.distance_to_neighbours.push_back(11);
	eight.distance_to_neighbours.push_back(1);
	eight.command.push_back("backward");
	eight.command.push_back("curve_forward_left");


	/*e.neighbours.push_back(&c);
	e.neighbours.push_back(&f);

	f.neighbours.push_back(&d);
	f.neighbours.push_back(&e);*/
	one.neighbours.push_back(&two);
	one.neighbours.push_back(&four);
	one.neighbours.push_back(&three);
	two.neighbours.push_back(&one);
	two.neighbours.push_back(&seven);
	three.neighbours.push_back(&one);
	three.neighbours.push_back(&eight);
	four.neighbours.push_back(&one);
	four.neighbours.push_back(&five);
	five.neighbours.push_back(&four);
	five.neighbours.push_back(&six);
	six.neighbours.push_back(&seven);
	six.neighbours.push_back(&five);
	six.neighbours.push_back(&eight);
	seven.neighbours.push_back(&two);
	seven.neighbours.push_back(&six);
	eight.neighbours.push_back(&three);
	eight.neighbours.push_back(&six);



	AI::A_Star pathfinder(one, six);
	std::vector<AI::node> path = pathfinder.search();

	return path;
}


//https://www.boost.org/doc/libs/1_52_0/doc/html/boost_asio/tutorial/tutdaytime3.html

//using namespace asio;
using asio::ip::tcp;
using std::string;
using std::cout;
using std::cin;
using std::endl;

int threadNo = 1;

std::mutex num_mutex;
std::mutex print_mutex;


//How to terminate write thread when read is terminated?? 
// Should I also terminate read with a write closed flag??
//make function to close socket? Will have to check if socket isn't closed??
//Maybe read shouldn't be able to close socket?




string read(tcp::socket& socket) {
	asio::streambuf buf;
	string data = "";
	asio::read_until(socket, buf, "\n");
	data = asio::buffer_cast<const char*>(buf.data());
	return data;

}

void send(tcp::socket& socket, const string& message) {
	const string msg = message + "\n";
	asio::write(socket, asio::buffer(message));
}


int valueOfThreadNum()
{
	int result = 0;
	{
		std::unique_lock<std::mutex> numLock(num_mutex);
		result = threadNo;
	}
	return result;
}

void handle_connection(tcp::socket sock, int id)
{
	{
		std::unique_lock<std::mutex> pLock(print_mutex);
		cout << "connected : " + id << endl;
	}



	if (id == 2)
	{
		{
			std::unique_lock<std::mutex> pLock(print_mutex);
			cout << "Sleeping : " + id << endl;
		}
		Sleep(10000);
	}
	else if (id == 3)
	{
		{
			std::unique_lock<std::mutex> pLock(print_mutex);
			cout << "Sleeping :# " + id << endl;
		}
		Sleep(10000);
	}

	int count = 0;
	send(sock, "Connection Establihed");

	{
		std::unique_lock<std::mutex> pLock(print_mutex);
		cout << "Connection Established with client : " + id << endl;
	}
	size_t bytes_readable = sock.available();
	asio::error_code error;
	std::vector<AI::node> path = createpath(id);
	while (true) {

		try {
			string message = "";
			bytes_readable = sock.available();
			if (bytes_readable > 0)
			{
				message = read(sock);
				{
					std::unique_lock<std::mutex> pLock(print_mutex);
					cout << "Message from client : " + id << endl;
					cout << message << endl;
				}
			}

			else
			{
				{
					std::unique_lock<std::mutex> pLock(print_mutex);
					cout << "Reading first node...will terminate when node = 0" << endl;
				}

				if (count >= path.size())
				{
					char answer = '5';

					{
						std::unique_lock<std::mutex> pLock(print_mutex);
						cout << id << " : Run again? reply with 1 for yes or 0 for no" << endl;
						cin >> answer;
						cout << "You inputted " << answer << endl;
						if (answer == '1') { cout << "IT'S ONE" << endl; }
						else if (answer == '0') { cout << "Don't you dare" << endl; }
					}

					if (answer == '1')
					{
						count = 0;
						cout << "IT'S ONE o" << endl;
					}
					else if (answer == '0')
					{
						message = "0";
						{
							std::unique_lock<std::mutex> pLock(print_mutex);
							cout << "Reached : " << path.at(count - 1).name << endl;
						}
					}

				}
				if (count < path.size() - 1)
				{
					int index = find_node_index_in_vector(path.at(count).neighbours, path.at(count + 1));
					message = path.at(count).command.at(index);
					{
						std::unique_lock<std::mutex> pLock(print_mutex);
						cout << "At node name : " << path.at(count).name << endl;
					}
				}

				{
					std::unique_lock<std::mutex> pLock(print_mutex);
					cout << "Node Read complete, Sending message : " << message << endl;
				}

				if (message == "0")
				{
					{
						std::unique_lock<std::mutex> pLock(print_mutex);
						cout << "Terminating..." << endl;
					}
					send(sock, "0");
					{
						std::unique_lock<std::mutex> numLock(num_mutex);
						threadNo--;
					}
					break;
				}

				else {
					{
						std::unique_lock<std::mutex> pLock(print_mutex);
						cout << "Sending message : " << message << endl;
					}

					send(sock, message);

				}

			}
			if (error == asio::error::eof)
			{
				{
					std::unique_lock<std::mutex> pLock(print_mutex);
					cout << "End of File!" << endl;
				}
			}
			else if (error)
			{
				{
					std::unique_lock<std::mutex> pLock(print_mutex);
					cout << "Error!" << endl;
				}

				throw asio::system_error(error); // Some other error.
			}
		}
		catch (std::exception& e) {
			std::cerr << id + " : Exception in thread: " << e.what() << " count: " << count << endl;
			{
				std::unique_lock<std::mutex> numLock(num_mutex);
				threadNo--;
				{
					std::unique_lock<std::mutex> pLock(print_mutex);
					cout << "Quiting, Thread No: " << threadNo << endl;
				}
			}
			break;
		}
		count++;
		Sleep(5000);
	}
}

int main()
{
	bool firstThreatCreated = false;

	printf("Booting up...");
	printf("\n");

	asio::io_service io_service;
	//io_service.run();
	//listen for new connection
	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));

	//waiting for connection	
	//New thread for each new connection
	std::vector<tcp::socket> sockets;
	sockets.push_back(tcp::socket (io_service));
	while (valueOfThreadNum() > 0)
	{


		//socket creation	
		//tcp::socket socket(io_service);
		sockets.push_back(tcp::socket(io_service));
		acceptor.async_accept(sockets.at(threadNo));


		{
			std::unique_lock<std::mutex> pLock(print_mutex);
			cout << "Someone connected! Creating thread for connection..." << endl;
		}

		//threadNo-1 because threadNo is initialised to 1 while all the method assume 0
		std::thread connection(handle_connection, std::move(sockets.at(threadNo)), threadNo);
		//connection.join();
		connection.detach();

		{
			std::unique_lock<std::mutex> numLock(num_mutex);
			if (firstThreatCreated == false) { firstThreatCreated == true; }
			else { threadNo++; }
			cout << "Thread No is " << threadNo << endl;
		}

		{
			std::unique_lock<std::mutex> pLock(print_mutex);
			cout << "Threads created for connection..." << endl;
		}
	}

	cout << "Terminating the server in 5 seconds" << endl;
	Sleep(5000);
	//std::terminate();

	return 0;
}
