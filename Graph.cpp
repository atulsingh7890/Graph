//
//  Graph.cpp
//  Cliques
//
//  Created by Atul Singh on 9/7/16.
//  Copyright (c) 2016 Atul. All rights reserved.
//  Ref : https://en.wikipedia.org/wiki/Bron%E2%80%93Kerbosch_algorithm
//  This is the C++ Version of the Document : http://www.kuchaev.com/files/graph.py
//  Currently i have ported only find_all_cliques , will be Porting all of other functions in C++ sooner
//

#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <stack>
#include <tuple>
#include <algorithm>
#include <iterator>
using namespace std;

class Graph
{
	
	
public:
	Graph()
	{
		
	}
	
	
	void addNode(int inNodeIndex)
	{
		if (mNodes.find(inNodeIndex) != mNodes.end())
			return;  //Throw Exception that Node already exists
		mNodes.insert(inNodeIndex);
		mAdjacencyList.insert(std::make_pair(inNodeIndex, std::set<int>()));
	}
	
	void addEdge(int inStartNode, int inEndNode)
	{
		if (mNodes.find(inStartNode) == mNodes.end())
		{
			cout << "Nodes doesn't adding the Start Node" << endl;
			mNodes.insert(inStartNode);
			mAdjacencyList.insert(std::make_pair(inStartNode, std::set<int>()));
		}
		if (mNodes.find(inEndNode) == mNodes.end())
		{
			cout << "Nodes doesn't adding the End Node" << endl;
			mNodes.insert(inEndNode);
			mAdjacencyList.insert(std::make_pair(inEndNode, std::set<int>()));
		}
		
		//Check if there is already in the Adjacency List
		if (mAdjacencyList.find(inStartNode) == mAdjacencyList.end())
		{
			mAdjacencyList.insert(std::make_pair(inStartNode, std::set<int>()));
		}
		else
		{
			mAdjacencyList.find(inStartNode)->second.insert(inEndNode);
		}
		
		if (mAdjacencyList.find(inEndNode) == mAdjacencyList.end())
		{
			mAdjacencyList.insert(std::make_pair(inEndNode, std::set<int>()));
		}
		else
		{
			mAdjacencyList.find(inEndNode)->second.insert(inStartNode);
		}
	}
	
	
	bool are_adjacent(int inNode1, int inNode2)
	{
		//Checks if Node1 and Node2 are adjacent
		return  mAdjacencyList[inNode1].find(inNode2) != mAdjacencyList[inNode1].end() ? true : false;
	}
	
	std::set<int> get_node_neighbours(int inNode)
	{
		return mAdjacencyList[inNode];
	}
	
	
	std::vector< std::set<int> > find_all_cliques(int min_vertex_in_clique=3)
	{
		//Implements Bron-Kerbosch algorithm , Version 2
		std::vector< std::set<int> > cliques;
		typedef std::tuple< std::set<int>, std::set<int>, std::set<int>, int, int   >  stackNode;
		std::stack< stackNode > stack_;
		int nd = -1;
		int disc_num = mNodes.size();
		stackNode search_node = std::make_tuple(set<int>(), set<int>(mNodes), set<int>(), nd, disc_num);
		stack_.push(search_node);
		while (stack_.size() > 0)
		{
			stackNode tupel = stack_.top();
			stack_.pop();
			
			set<int> c_compsub = std::get<0>(tupel);
			set<int> c_candidates = std::get<1>(tupel);
			set<int> c_not = std::get<2>(tupel);
			int c_nd = std::get<3>(tupel);
			int c_disc_cum = std::get<4>(tupel);
			
			if (c_candidates.size() == 0 && c_not.size() == 0)
			{
				//Here we are pushing the Cliques
				if (c_compsub.size() >= min_vertex_in_clique)
				{
					//Before Pushing Back check if it already exists or not
					bool alreadyPresent = false;
					for (int i = 0; i < cliques.size(); ++i)
					{
						if (cliques[i].size() == c_compsub.size())
						{
							bool setMatched = true;
							set<int>::iterator it1, it2;
							for (it1 = cliques[i].begin(), it2 = c_compsub.begin();
								 it1 != cliques[i].end() && it2 != c_compsub.end();
								 ++it1, ++it2 )
							{
								if ( (*it1) != (*it2))
								{
									setMatched = false;
									break;
								}
							}
							
							if (setMatched)
							{
								alreadyPresent = true;
								break;
							}
						}
					}
					
					if (false ==  alreadyPresent)
						cliques.push_back( std::set<int>(c_compsub));
					continue;
				}
			}
			
			std::set<int>::iterator it = c_candidates.begin();
			while ( it != c_candidates.end() )
			{
				int u = *it;
				it++;
				
				if ((c_nd == -1) || !(are_adjacent(u, c_nd)))
				{
					c_candidates.erase(u);
					std::set<int> Nu = get_node_neighbours(u);
					std::set<int> new_compsub = set<int>(c_compsub);
					new_compsub.insert(u);
					set<int> new_candidates;
					set_intersection(c_candidates.begin(), c_candidates.end(), Nu.begin(), Nu.end(), std::inserter(new_candidates, new_candidates.begin()));
					
					set<int> new_not;
					set_intersection(c_not.begin(), c_not.end(), Nu.begin(), Nu.end(), std::inserter(new_not, new_not.begin()));
					
					if (c_nd != -1)
					{
						if (new_not.find(c_nd) != new_not.end())
						{
							int new_disc_num = c_disc_cum - 1;
							if (new_disc_num > 0)
							{
								stackNode new_search_node = std::make_tuple(new_compsub, new_candidates, new_not, c_nd, new_disc_num);
								stack_.push(new_search_node);
							}
						}
						else
						{
							int new_disc_num = mNodes.size();
							int new_nd = c_nd;
							for (int cand_nd : new_not)
							{
								set<int> tempSet;
								set<int> tempNeightBours = get_node_neighbours(c_nd);
								set_intersection(new_candidates.begin(), new_candidates.end(),
												 tempNeightBours.begin(), tempNeightBours.end(),
												 std::inserter(tempSet, tempSet.begin()));
								
								int cand_disc_num = new_candidates.size() - tempSet.size();
								
								if (cand_disc_num < new_disc_num)
								{
									new_disc_num = cand_disc_num;
									new_nd = cand_nd;
								}
							}
							
							stackNode new_search_node = std::make_tuple(new_compsub, new_candidates, new_not, new_nd, new_disc_num);
							stack_.push(new_search_node);
						}
					}
					else
					{
						stackNode new_search_node = std::make_tuple(new_compsub, new_candidates, new_not, c_nd, c_disc_cum);
						stack_.push(new_search_node);
					}
					
					c_not.insert(u);
					int new_disc_num = 0;
					for (int x : c_candidates)
					{
						if (false == are_adjacent(x, u))
						{
							new_disc_num += 1;
						}
					}
					
					if (new_disc_num < c_disc_cum && new_disc_num > 0)
					{
						stackNode new1_search_node = std::make_tuple(c_compsub, c_candidates, c_not, u, new_disc_num);
						stack_.push(new1_search_node);
					}
					else
					{
						stackNode new1_search_node = std::make_tuple(c_compsub, c_candidates, c_not, c_nd, c_disc_cum);
						stack_.push(new1_search_node);
						
					}
				}
				
			}
		}
		
		
		return cliques;
		
	}
	
private:
	std::set<int> mNodes;
	std::map< int, std::set<int> > mAdjacencyList;
	
	
};

int main()
{
	Graph g = Graph();
	g.addNode(0);
	g.addNode(1);
	g.addNode(2);
	g.addNode(3);
	g.addNode(4);
	g.addNode(5);
	
	
	g.addEdge(0, 1);
	g.addEdge(0, 2);
	g.addEdge(1, 2);
	g.addEdge(1, 3);
	g.addEdge(3, 4);
	g.addEdge(5, 4);
	g.addEdge(3, 5);
	
	cout << g.are_adjacent(1, 3);
	cout << g.are_adjacent(1, 4);
	
	std::vector< std::set<int >  > cliques = g.find_all_cliques();
	
}