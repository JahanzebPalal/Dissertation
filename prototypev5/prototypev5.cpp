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
using asio::ip::tcp;
using std::string;
using std::cout;
using std::cin;
using std::endl;



AI::node one;
AI::node two;
AI::node three;
AI::node four;
AI::node five;
AI::node six;
AI::node seven;
AI::node eight;

bool first_reached = false;
bool second_reached = false;

int threadNo = 1;

std::mutex num_mutex;
std::mutex print_mutex;


int find_node_index_in_vector(std::vector<AI::node*> vector, AI::node& candidate)
{
	bool stop = false;
	//For debugging
	if (candidate.name == "two") { stop = true;}

	printf("Finding node in vector : server... \n");
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

std::vector<AI::node> createpath()
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

	one.time_to_neighbours_in_secs.push_back(1);
	one.time_to_neighbours_in_secs.push_back(2);
	one.time_to_neighbours_in_secs.push_back(1);
	one.command.push_back("curve_forward_right");
	one.command.push_back("forward");
	one.command.push_back("curve_forward_left");


	two.time_to_neighbours_in_secs.push_back(10000000);
	two.time_to_neighbours_in_secs.push_back(1);
	two.command.push_back("backward");
	two.command.push_back("forward");


	three.time_to_neighbours_in_secs.push_back(100000000);
	three.time_to_neighbours_in_secs.push_back(2);
	three.command.push_back("backward");
	three.command.push_back("forward");


	four.time_to_neighbours_in_secs.push_back(10000000);
	four.time_to_neighbours_in_secs.push_back(5);
	four.command.push_back("backward");
	four.command.push_back("forward");

	
	five.time_to_neighbours_in_secs.push_back(4);
	five.time_to_neighbours_in_secs.push_back(2);
	five.command.push_back("backward");
	five.command.push_back("forward");

		
	six.time_to_neighbours_in_secs.push_back(4);
	six.time_to_neighbours_in_secs.push_back(1);


	seven.time_to_neighbours_in_secs.push_back(5);
	seven.time_to_neighbours_in_secs.push_back(3);
	seven.command.push_back("backward");
	seven.command.push_back("forward");

	
	eight.time_to_neighbours_in_secs.push_back(100000);
	eight.time_to_neighbours_in_secs.push_back(100000);
	eight.time_to_neighbours_in_secs.push_back(100000);
	eight.command.push_back("backward");
	eight.command.push_back("backward");
	eight.command.push_back("backward");




	one.directional_heuristic = 1000000;
	two.directional_heuristic = 100000;
	three.directional_heuristic = 7;
	four.directional_heuristic = 5;
	five.directional_heuristic = 12;
	six.directional_heuristic = 6;
	seven.directional_heuristic = 9;
	eight.directional_heuristic = 0;




	one.neighbours.push_back(&two);
	one.neighbours.push_back(&three);
	one.neighbours.push_back(&four);

	two.neighbours.push_back(&one);
	two.neighbours.push_back(&five);

	three.neighbours.push_back(&one);
	three.neighbours.push_back(&six);

	four.neighbours.push_back(&one);
	four.neighbours.push_back(&seven);

	five.neighbours.push_back(&two);
	five.neighbours.push_back(&eight);

	six.neighbours.push_back(&three);
	six.neighbours.push_back(&eight);

	seven.neighbours.push_back(&four);
	seven.neighbours.push_back(&eight);

	eight.neighbours.push_back(&five);
	eight.neighbours.push_back(&six);
	eight.neighbours.push_back(&seven);



	AI::A_Star pathfinder(one, eight);
	std::vector<AI::node> path = pathfinder.search();

	return path;
}


//https://www.boost.org/doc/libs/1_52_0/doc/html/boost_asio/tutorial/tutdaytime3.html

//using namespace asio;





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
	const string msg = message;
	asio::write(socket, asio::buffer(msg));
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

	send(sock, "Connection Establihed : " + std::to_string(id));
	send(sock, "thread id : " + std::to_string(id));

	if (id == 1)
	{
		bool start = false;
		char answer = '5';
		while (start != true) 
		{ 
			{
				std::unique_lock<std::mutex> pLock(print_mutex);
				cout << id << " : Start? reply with 1 for yes" << endl;
				cin >> answer;
				if (answer == '1') { cout << "Starting" << endl; start = true;}
				else { send(sock, "Waiting");}
			}
		}
	}

	if (id == 2) 
	{ 
		while (first_reached != true) { send(sock, "Waiting"); Sleep(3000);}
	}
	else if (id == 3) { 
		while (second_reached != true) { send(sock, "Waiting"); Sleep(3000); }
	}

	int count = 0;

	/*{
		std::unique_lock<std::mutex> pLock(print_mutex);
		cout << "Connection Established with client : " + id << endl;
	}*/
	
	asio::error_code error;
	std::vector<AI::node> path = createpath();

	while (true) {




		try {
			string message = "";
				
				/*{
					std::unique_lock<std::mutex> pLock(print_mutex);
					cout << "Reading first node...will terminate when node = 0" << endl;
				}*/

				if (count >= path.size()) 
				{ 
					{
					std::unique_lock<std::mutex> pLock(print_mutex);
					cout << "Starting next trilobot" << endl;
					if (id == 1) { first_reached = true; }
					else if (id == 2) { second_reached = true; }
					}
					send(sock, "0");
					

					break;
					//char answer = '5';

					//{
					//	std::unique_lock<std::mutex> pLock(print_mutex);
					//	cout << id <<" : Run again? reply with 1 for yes or 0 for no" << endl;
					//	cin >> answer;
					//	/*cout << "You inputted " << answer << endl;
					//	if (answer == '1') { cout<<"IT'S ONE"<<endl;}
					//	else if (answer == '0') {cout<< "Don't you dare"<<endl;}*/
					//}


					// if (answer == '1')
					// {
					//	count = 0;
					//	cout << "Resetting : " << id << endl;
					//	if (id == 1) {first_reached = false; second_reached = false;}

					// }
					//else if (answer == '0')
					//{
					//	message = "0";
					//	{
					//		std::unique_lock<std::mutex> pLock(print_mutex);
					//		cout << "Answer was no" << endl;
					//	}
					//}

				}


				if (count<path.size()-1)
				{
					int index = find_node_index_in_vector(path.at(count).neighbours, path.at(count + 1));
					message = path.at(count).command.at(index);
					/*{
						std::unique_lock<std::mutex> pLock(print_mutex);
						cout << "At node name : " << path.at(count).name << endl;
					}*/

					send(sock, "At node name :" + path.at(count).name);
				}
				else if (count == path.size() - 1)
				{
					//if (id == 1) { first_reached = true; }
					//else if (id == 2) { second_reached = true; }

					/*{
						std::unique_lock<std::mutex> pLock(print_mutex);
						cout << "Reached : " << path.at(count - 1).name << endl;
					}*/

					send(sock, "Reached : " + path.at(count-1).name);

				}

				/*{
					std::unique_lock<std::mutex> pLock(print_mutex);
					cout << "Node Read complete, Sending message : " << message << endl;
				}*/

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

				else 
				{
					{
						std::unique_lock<std::mutex> pLock(print_mutex);
						cout << id << " : Sending message : " << message << endl;
					}
					send(sock, message);
				}

			
			/*if (error == asio::error::eof)
			{
				{
					std::unique_lock<std::mutex> pLock(print_mutex);
					cout << "End of File!" << endl;
				}
			}*/
			//else 
			if (error)
			{
				{
					std::unique_lock<std::mutex> pLock(print_mutex);
					cout << "Error!" << endl;
				}
				{
					std::unique_lock<std::mutex> numLock(num_mutex);
					threadNo--;
				}
				throw asio::system_error(error); // Some other error.
			}
		}
		catch (std::exception& e) {
			std::cerr << id + " : Exception in thread: " << e.what() << " count: " << count <<endl;
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
		Sleep(3000);
	}
}





void accept_connections(asio::io_context& context, tcp::acceptor& acceptor) {
	//asio::ip::tcp::socket socket(context);

	// Accept a new client connection asynchronously

	acceptor.async_accept([&](const std::error_code err, asio::ip::tcp::socket socket)
		{
			if (!err)
			{
				{
					std::unique_lock<std::mutex> pLock(print_mutex);
					cout << "Someone connected! Creating thread for connection for "<< threadNo << "..." << endl;
				}
				std::thread t(handle_connection, std::move(socket), threadNo);
				t.detach();
				threadNo++;
				{
					std::unique_lock<std::mutex> pLock(print_mutex);
					cout << "Thread Created" << endl;
				}

			}

			accept_connections(context, acceptor);
		});
}





int main()
{
	//bool firstThreatCreated = false;

	printf("Booting up...");
	printf("\n");

	//asio::io_service io_service;
	//context.run();
	//listen for new connection

	asio::io_context context;
	asio::ip::tcp::acceptor acceptor(context, tcp::endpoint(tcp::v4(), 1234));

	try 
	{
		accept_connections(context, acceptor);
		context.run();
	}
	catch(std::system_error& e)
	{
		std::cerr << "Error: " << e.what() << endl;
	}

			
	//while (true)
	//{
	//
	//	//std::thread connection(handle_connection, std::move(socket), threadNo);
	//	//connection.join();
	//	//connection.detach();
	//	
	//}

	cout << "Terminating the server in 5 seconds" << endl;
	Sleep(5000);
	//std::terminate();

	return 0;
}
