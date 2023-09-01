#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <windows.h>


//Each node has it's own command to the trilobot???
//i.e to read node B from node A only the forward comand is required...



namespace AI
{
	struct node
	{
		bool start = false;
		std::string name;

		std::vector <node*> neighbours;

		//All the nodes passed through to reach this node at the lowest possible cost.
		std::string parent = "";

		//std::vector <int> directional_heuristic;
		int directional_heuristic = 0;
		std::vector <int> time_to_neighbours_in_secs;
		//lowest cost to reach the node. As high as possible in the beginning to so the A* algorithm works
		int cost_to_reach = 0;
		std::vector <std::string> command;

	};

	class A_Star
	{


		//std::vector<node*> map;
		node  begin;
		node  finish;
	public:
		//A_Star(std::vector<node*> &grid, node * start, node * end)
		A_Star(node start, node end)
		{
			//map = grid;
			begin = start;
			finish = end;
			printf("Object created...");

			std::cout << "Starting : " << begin.name << std::endl;
			std::cout << "Ending : " << end.name << std::endl;


		}
		//Calculate the commulative cost of each generation of parents before the current node.
		//Use std::find to find the token taken??
		/*int calculate_cost_so_far(std::vector <node> &parents, std::vector<std::string> names)
		{

			int result = 0;

			for (int i=0; i<parents.size(); i++)
			{

			}
		}*/

		node search_lowest_total_cost(std::vector<node>& candidates, node& previous)
		{
			node pNode = candidates.at(0);
			std::cout << " node with lowest cost : " << pNode.name << std::endl;
			int lowest = 10000000;
			for (int i = 0; i < candidates.size(); i++)
			{
				if (((candidates.at(i).cost_to_reach + candidates.at(i).directional_heuristic) < lowest)
					//&& (candidates.at(i).start == false)
					)
				{

					lowest = candidates.at(i).cost_to_reach;
					pNode = candidates.at(i);

					std::cout << " node with lowest cost : loop : " << pNode.name << std::endl;
				}
			}
			//std::cout << " node with lowest cost : " << pNode.name << std::endl;
			return pNode;
		}

		bool find_node_in_vector(std::vector<node> vector, node& candidate)
		{
			bool stop = false;
			printf("Finding node in vector : Bool...");
			if (candidate.name == "f") { stop = true; }
			for (int i = 0; i < vector.size(); i++)
			{
				if (vector.at(i).name == candidate.name)
				{
					return true;
				}
			}
			return false;
		}


		int find_node_index_in_vector(std::vector<node> vector, node& candidate)
		{
			bool stop = false;
			if (candidate.name == "f") { stop = true; }

			printf("Finding node in vector : Bool...");
			int index = 0;
			for (int i = 0; i < vector.size(); i++)
			{
				if (vector.at(i).name == candidate.name)
				{
					return i;
				}
			}
			return index;
		}

		node find_parent_in_vector(std::vector<node> vector, node& candidate)
		{
			printf("Finding parent in vector : node&...\n");

			for (int i = 0; i < vector.size(); i++)
			{
				if (vector.at(i).name == candidate.parent)
				{
					node output = vector.at(i);
					return output;
				}
			}
			return candidate;
		}



		std::vector<node> generate_best_path(std::vector<node>& vector, node& end)
		{
			std::vector<node> path;
			node input = end;
			path.push_back(input);
			bool term = false;

			while (term != true)
			{
				if (end.name != find_parent_in_vector(vector, input).name)
				{
					input = (find_parent_in_vector(vector, input));
					path.insert(path.begin(), input);
					if (path.at(0).start == true) { term = true; }
				}
			}

			return path;
		}

		void sort_openlist(std::vector<node>& openlist)
		{
			int index = 0;
			node temp;
			for (int i = 0; i < openlist.size(); i++)
			{
				index = i;
				for (int j = i + 1; j < openlist.size(); j++)
				{
					if (openlist.at(j).cost_to_reach + openlist.at(j).directional_heuristic < openlist.at(index).cost_to_reach + openlist.at(index).directional_heuristic)
					{
						index = j;
					}
				}
				temp = openlist.at(i);
				openlist.at(i) = openlist.at(index);
				openlist.at(index) = temp;
			}


		}

		//Add up the heuristics at the end of the path??
		std::vector<node> search()
		{
			std::vector<node> best_path;

			std::vector<node> unvisited;
			std::vector<node> visited;

			node& current_node = begin;
			node previous;

			std::vector<node> allnodes;
			bool stop = false;

			visited.push_back(begin);
			unvisited.push_back(begin);
			visited.at(0).cost_to_reach = 0;

			while (!unvisited.empty())
			{
				previous = current_node;
				sort_openlist(unvisited);
				current_node = unvisited.front();
				std::cout << "At node: " << current_node.name << std::endl;

				if (current_node.name == "f") { stop = true; }

				for (int i = 0; i < current_node.neighbours.size(); i++)
				{
					std::cout << "At node: " << current_node.name << " checking neighbour : " << current_node.neighbours.at(i)->name << std::endl;

					if ((find_node_in_vector(unvisited, *current_node.neighbours.at(i)) == false)
						&& (find_node_in_vector(visited, *current_node.neighbours.at(i)) == false)
						)
					{
						current_node.neighbours.at(i)->cost_to_reach = current_node.cost_to_reach + current_node.time_to_neighbours_in_secs.at(i);
						current_node.neighbours.at(i)->parent = current_node.name;
						unvisited.push_back(*current_node.neighbours.at(i));
					}
					else if ((find_node_in_vector(unvisited, *current_node.neighbours.at(i)) == true)
						&& (unvisited.at(find_node_index_in_vector(unvisited, *current_node.neighbours.at(i))).cost_to_reach
				> current_node.cost_to_reach + current_node.time_to_neighbours_in_secs.at(i))
						)
					{
						unvisited.at(find_node_index_in_vector(unvisited, *current_node.neighbours.at(i))).cost_to_reach
							= current_node.cost_to_reach + current_node.time_to_neighbours_in_secs.at(i);
						current_node.neighbours.at(i)->cost_to_reach =
							current_node.cost_to_reach + current_node.time_to_neighbours_in_secs.at(i);


						unvisited.at(find_node_index_in_vector(unvisited, *current_node.neighbours.at(i))).parent
							= current_node.name;
						current_node.neighbours.at(i)->name
							= current_node.name;

					}

					else if ((find_node_in_vector(visited, *current_node.neighbours.at(i)) == true)
						&& (visited.at(find_node_index_in_vector(visited, *current_node.neighbours.at(i))).cost_to_reach
					> current_node.cost_to_reach + current_node.time_to_neighbours_in_secs.at(i))
						)
					{
						visited.at(find_node_index_in_vector(visited, *current_node.neighbours.at(i))).cost_to_reach
							= current_node.cost_to_reach + current_node.time_to_neighbours_in_secs.at(i);

						current_node.neighbours.at(i)->cost_to_reach =
							current_node.cost_to_reach + current_node.time_to_neighbours_in_secs.at(i);

						visited.at(find_node_index_in_vector(visited, *current_node.neighbours.at(i))).parent
							= current_node.name;

						current_node.neighbours.at(i)->name
							= current_node.name;
					}
				}

				visited.push_back(current_node);
				unvisited.erase(unvisited.begin() + find_node_index_in_vector(unvisited, current_node));
			}

			for (int i = 0; i < visited.size(); ++i)
			{
				if (finish.name == visited.at(i).name) { finish = visited.at(i); }
			}
			best_path = generate_best_path(visited, finish);
			return best_path;
		}


		~A_Star()
		{

		};
		//End of class
	};

	//End of namespace
};

//int notmain()
//{
//	printf("Creating nodes...\n");
//	AI::node a;
//	AI::node b;
//	AI::node c;
//	AI::node d;
//	AI::node e;
//	AI::node f;
//	AI::node g;
//
//	//A setup
//	a.distance_to_neighbours.push_back(3);
//	a.distance_to_neighbours.push_back(7);
//	a.name = "a";
//	a.parent = "start";
//	a.start = true;
//	a.cost_to_reach = 100000;
//
//	//B setpup
//
//	//Give A an extremely high weight to it isn't backtracked too
//	//This is because A is the starting node
//	b.distance_to_neighbours.push_back(1000000);
//	b.distance_to_neighbours.push_back(5);
//	b.distance_to_neighbours.push_back(11);
//	b.name = "b";
//
//	//C setup
//	//Give A an extremely high weight to it isn't backtracked too. This is because A is the starting node
//	c.distance_to_neighbours.push_back(1000000);
//	c.distance_to_neighbours.push_back(5);
//	c.name = "c";
//
//	//D setup
//	d.distance_to_neighbours.push_back(5);
//	d.distance_to_neighbours.push_back(1);
//	d.name = "d";
//
//	//E setup
//	e.distance_to_neighbours.push_back(1);
//	e.distance_to_neighbours.push_back(5);
//	e.name = "e";
//
//	//F setup
//	f.distance_to_neighbours.push_back(100000000);
//	f.distance_to_neighbours.push_back(100000000);
//	f.distance_to_neighbours.push_back(100000000);
//	f.name = "f";
//
//	//G setup
//	g.distance_to_neighbours.push_back(5);
//	g.distance_to_neighbours.push_back(1);
//	g.name = "g";
//
//	//Setup neighbours
//	a.neighbours.push_back(&b);
//	a.neighbours.push_back(&c);
//
//	b.neighbours.push_back(&a);
//	b.neighbours.push_back(&d);
//	b.neighbours.push_back(&f);
//
//	c.neighbours.push_back(&a);
//	c.neighbours.push_back(&g);
//
//	d.neighbours.push_back(&b);
//	d.neighbours.push_back(&e);
//
//	e.neighbours.push_back(&d);
//	e.neighbours.push_back(&f);
//
//	g.neighbours.push_back(&c);
//	g.neighbours.push_back(&f);
//
//
//	f.neighbours.push_back(&b);
//	f.neighbours.push_back(&e);
//	f.neighbours.push_back(&g);
//
//
//	//Setting up directional heuristics
//	/*a.directional_heuristic = 100000000;
//	b.directional_heuristic = 12;
//	c.directional_heuristic = 8;
//	d.directional_heuristic = 5;
//	e.directional_heuristic = 2;
//	g.directional_heuristic = 4;
//	f.directional_heuristic = 0;*/
//
////sort test
//	a.cost_to_reach = 0;
//	b.cost_to_reach = 1000000000000;
//	c.cost_to_reach = 1000000000000;
//	d.cost_to_reach = 1000000000000;
//	e.cost_to_reach = 1000000000000;
//	f.cost_to_reach = 1000000000000;
//	g.cost_to_reach = 1000000000000;
//
//
//	//std::vector<AI::node> openlist;
//
//	//openlist.push_back(g);
//	//openlist.push_back(f);
//	//openlist.push_back(e);
//	//openlist.push_back(d);
//	//openlist.push_back(c);
//	//openlist.push_back(b);
//	//openlist.push_back(a);
//
//
//	/*printf("Searching best path... \n");
//	printf("Creating Object... \n");
//	AI::A_Star pathfinder(a, f);
//	printf("Searching... \n");
//	AI::path best_path = pathfinder.search();*/
//
//	
//	AI::A_Star pathfinder(a, f);
//	std::vector<AI::node> path = pathfinder.search();
//
//	/*for (int i = 0; i < openlist.size(); i++)
//	{
//		std::cout << openlist.at(i).name << std::endl;
//	}*/
//
//
//	printf("Displaying best path...\n");
//	for (int i = 0; i < path.size(); i++)
//	{
//		std::string output = path.at(i).name;
//		std::cout << output << std::endl;
//	}
//
//	/*for (int i = 0; i < openlist.size(); i++)
//	{
//		std::cout << openlist.at(i).name << std::endl;
//	}*/
//	return 0;
//}