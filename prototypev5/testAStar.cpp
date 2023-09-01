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
	if (candidate.name == "two") { stop = true; }

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
	two.directional_heuristic = 10;
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

int main() 
{

	std::vector<AI::node> path = createpath();

	std::cout << "loop" << std::endl;
	system("cls");
	for (int i=0; i<path.size()-1;i++ )
	{
		std::cout << path.at(i).name << std::endl;
		//int index = find_node_index_in_vector(path.at(i).neighbours, path.at(i + 1));
		//std::string message = path.at(i).command.at(index);
		//std::cout << message << std::endl;
	}
	std::cout << "eight" << std::endl;
	return 0;
}