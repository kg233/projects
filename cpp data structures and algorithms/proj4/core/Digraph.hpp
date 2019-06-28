// Digraph.hpp
//
// ICS 46 Winter 2019
// Project #4: Rock and Roll Stops the Traffic
//
// This header file declares a class template called Digraph, which is
// intended to implement a generic directed graph. The implementation
// uses the adjacency lists technique, so each vertex stores a linked
// list of its outgoing edges.
//
// Along with the Digraph class template is a class DigraphException
// and a couple of utility structs that aren't generally useful outside
// of this header file.
//
// In general, directed graphs are all the same, except in the sense
// that they store different kinds of information about each vertex and
// about each edge; these two types are the type parameters to the
// Digraph class template.

#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <exception>
#include <functional>
#include <list>
#include <map>
#include <utility>
#include <vector>

#include <algorithm>
#include <unordered_set>
#include <queue>

#include <iostream>



// DigraphExceptions are thrown from some of the member functions in the
// Digraph class template, so that exception is declared here, so it
// will be available to any code that includes this header file.

// bool compare_less(std::pair<int, int> one, std::pair<int, int> another_one){
//     return one.second > another_one.second;
// }


class DigraphException : public std::runtime_error
{
public:
    DigraphException(const std::string& reason);
};


inline DigraphException::DigraphException(const std::string& reason)
    : std::runtime_error{reason}
{
}



// A DigraphEdge lists a "from vertex" (the number of the vertex from which
// the edge points), a "to vertex" (the number of the vertex to which the
// edge points), and an EdgeInfo object.  Because different kinds of Digraphs
// store different kinds of edge information, DigraphEdge is a struct template.

template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;
};



// A DigraphVertex includes two things: a VertexInfo object and a list of
// its outgoing edges.  Because different kinds of Digraphs store different
// kinds of vertex and edge information, DigraphVertex is a struct template.

template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
};



// Digraph is a class template that represents a directed graph implemented
// using adjacency lists.  It takes two type parameters:
//
// * VertexInfo, which specifies the kind of object stored for each vertex
// * EdgeInfo, which specifies the kind of object stored for each edge
//
// You'll need to implement the member functions declared here; each has a
// comment detailing how it is intended to work.
//
// Each vertex in a Digraph is identified uniquely by a "vertex number".
// Vertex numbers are not necessarily sequential and they are not necessarily
// zero- or one-based.

template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:
    // The default constructor initializes a new, empty Digraph so that
    // contains no vertices and no edges.
    Digraph();

    // The copy constructor initializes a new Digraph to be a deep copy
    // of another one (i.e., any change to the copy will not affect the
    // original).
    Digraph(const Digraph& d);

    // The move constructor initializes a new Digraph from an expiring one.
    Digraph(Digraph&& d) noexcept;

    // The destructor deallocates any memory associated with the Digraph.
    ~Digraph() noexcept;

    // The assignment operator assigns the contents of the given Digraph
    // into "this" Digraph, with "this" Digraph becoming a separate, deep
    // copy of the contents of the given one (i.e., any change made to
    // "this" Digraph afterward will not affect the other).
    Digraph& operator=(const Digraph& d);

    // The move assignment operator assigns the contents of an expiring
    // Digraph into "this" Digraph.
    Digraph& operator=(Digraph&& d) noexcept;

    // vertices() returns a std::vector containing the vertex numbers of
    // every vertex in this Digraph.
    std::vector<int> vertices() const;

    // edges() returns a std::vector of std::pairs, in which each pair
    // contains the "from" and "to" vertex numbers of an edge in this
    // Digraph.  All edges are included in the std::vector.
    std::vector<std::pair<int, int>> edges() const;

    // This overload of edges() returns a std::vector of std::pairs, in
    // which each pair contains the "from" and "to" vertex numbers of an
    // edge in this Digraph.  Only edges outgoing from the given vertex
    // number are included in the std::vector.  If the given vertex does
    // not exist, a DigraphException is thrown instead.
    std::vector<std::pair<int, int>> edges(int vertex) const;

    // vertexInfo() returns the VertexInfo object belonging to the vertex
    // with the given vertex number.  If that vertex does not exist, a
    // DigraphException is thrown instead.
    VertexInfo vertexInfo(int vertex) const;

    // edgeInfo() returns the EdgeInfo object belonging to the edge
    // with the given "from" and "to" vertex numbers.  If either of those
    // vertices does not exist *or* if the edge does not exist, a
    // DigraphException is thrown instead.
    EdgeInfo edgeInfo(int fromVertex, int toVertex) const;

    // addVertex() adds a vertex to the Digraph with the given vertex
    // number and VertexInfo object.  If there is already a vertex in
    // the graph with the given vertex number, a DigraphException is
    // thrown instead.
    void addVertex(int vertex, const VertexInfo& vinfo);

    // addEdge() adds an edge to the Digraph pointing from the given
    // "from" vertex number to the given "to" vertex number, and
    // associates with the given EdgeInfo object with it.  If one
    // of the vertices does not exist *or* if the same edge is already
    // present in the graph, a DigraphException is thrown instead.
    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo);

    // removeVertex() removes the vertex (and all of its incoming
    // and outgoing edges) with the given vertex number from the
    // Digraph.  If the vertex does not exist already, a DigraphException
    // is thrown instead.
    void removeVertex(int vertex);

    // removeEdge() removes the edge pointing from the given "from"
    // vertex number to the given "to" vertex number from the Digraph.
    // If either of these vertices does not exist *or* if the edge
    // is not already present in the graph, a DigraphException is
    // thrown instead.
    void removeEdge(int fromVertex, int toVertex);

    // vertexCount() returns the number of vertices in the graph.
    int vertexCount() const noexcept;

    // edgeCount() returns the total number of edges in the graph,
    // counting edges outgoing from all vertices.
    int edgeCount() const noexcept;

    // This overload of edgeCount() returns the number of edges in
    // the graph that are outgoing from the given vertex number.
    // If the given vertex does not exist, a DigraphException is
    // thrown instead.
    int edgeCount(int vertex) const;

    // isStronglyConnected() returns true if the Digraph is strongly
    // connected (i.e., every vertex is reachable from every other),
    // false otherwise.
    bool isStronglyConnected() const;

    // findShortestPaths() takes a start vertex number and a function
    // that takes an EdgeInfo object and determines an edge weight.
    // It uses Dijkstra's Shortest Path Algorithm to determine the
    // shortest paths from the start vertex to every other vertex
    // in the graph.  The result is returned as a std::map<int, int>
    // where the keys are vertex numbers and the value associated
    // with each key k is the precedessor of that vertex chosen by
    // the algorithm.  For any vertex without a predecessor (e.g.,
    // a vertex that was never reached, or the start vertex itself),
    // the value is simply a copy of the key.
    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;

    // void visualize();


private:
    // Add whatever member variables you think you need here.  One
    // possibility is a std::map where the keys are vertex numbers
    // and the values are DigraphVertex<VertexInfo, EdgeInfo> objects.
    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> map;

    // std::set<DigraphEdge<EdgeInfo>> edgeSet;
    int edge_Count;



    // You can also feel free to add any additional member functions
    // you'd like (public or private), so long as you don't remove or
    // change the signatures of the ones that already exist.

    bool exists_in_map(int vertex) const;

    unsigned int DFT_one(int Vertex) const;
    void DFTr(int vertex, std::unordered_set<int>& visited_set) const;

    // bool compare_less(std::pair<int, int> one, std::pair<int, int> another_one);
};



// You'll need to implement the member functions below.  There's enough
// code in place to make them compile, but they'll all need to do the
// correct thing instead.

template <typename VertexInfo, typename EdgeInfo> //default constructor
Digraph<VertexInfo, EdgeInfo>::Digraph()
:map{}, edge_Count{0}
{
}


template <typename VertexInfo, typename EdgeInfo> //copy constructor
Digraph<VertexInfo, EdgeInfo>::Digraph(const Digraph& d)
:map{d.map}, edge_Count{d.edgeCount()}
{
	// map = ;
}


template <typename VertexInfo, typename EdgeInfo> //move constructor
Digraph<VertexInfo, EdgeInfo>::Digraph(Digraph&& d) noexcept
:map{std::move(d.map)}, edge_Count{d.edgeCount()}
{
}


template <typename VertexInfo, typename EdgeInfo> //destructor
Digraph<VertexInfo, EdgeInfo>::~Digraph() noexcept
{
}


template <typename VertexInfo, typename EdgeInfo> //assignment operator
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(const Digraph& d)
{
	if (this != &d){
		edge_Count = d.edgeCount();
		map = d.map;
	}

    return *this;
}


template <typename VertexInfo, typename EdgeInfo> //assignment move operator
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(Digraph&& d) noexcept
{
	if (this != &d){
		edge_Count = d.edgeCount();
		std::swap(map, d.map);
	}
    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<int> Digraph<VertexInfo, EdgeInfo>::vertices() const
{
	std::vector<int> result;
	for (auto i = map.begin(); i != map.end(); i++){
		result.push_back(i->first);
	}
    return result;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges() const
{
    std::vector<std::pair<int, int>> result;
	for (auto i = map.begin(); i != map.end(); i++){
		for (auto x = i->second.edges.begin(); x != i->second.edges.end(); x++){
			result.push_back(std::pair<int, int>{i->first, x->toVertex});
		}
		
	}
	return result;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges(int vertex) const
{
	if (not exists_in_map(vertex)){
		throw DigraphException("vertex not found");
	}
	std::vector<std::pair<int, int>> result;
	auto target = map.at(vertex);
	for (auto x = target.edges.begin(); x != target.edges.end(); x++){
		result.push_back(std::pair<int, int>{vertex, x->toVertex});
	}
	return result;
}


template <typename VertexInfo, typename EdgeInfo>
VertexInfo Digraph<VertexInfo, EdgeInfo>::vertexInfo(int vertex) const
{
    if (not exists_in_map(vertex)){
		throw DigraphException("vertex not found");
	}
	return map.at(vertex).vinfo;
}


template <typename VertexInfo, typename EdgeInfo>
EdgeInfo Digraph<VertexInfo, EdgeInfo>::edgeInfo(int fromVertex, int toVertex) const
{
	if (not exists_in_map(toVertex) or not exists_in_map(fromVertex)){
		throw DigraphException("vertex not found");
	}
    auto target = map.at(fromVertex);
    auto pred = [toVertex](const DigraphEdge<EdgeInfo>& item){return item.toVertex == toVertex;};
	auto result = std::find_if(target.edges.begin(), target.edges.end(), pred);
	if (result == target.edges.end()){
		throw DigraphException("Edge not exist");
	}
	return result->einfo;

}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addVertex(int vertex, const VertexInfo& vinfo)
{
	auto value = map.insert(std::pair<int, DigraphVertex<VertexInfo, EdgeInfo>>{vertex, DigraphVertex<VertexInfo, EdgeInfo>{vinfo, std::list<DigraphEdge<EdgeInfo>>{}}});
	if (not value.second){
		throw DigraphException("vertex already exists");
	}
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo)
{
	if (not exists_in_map(toVertex)){
		throw DigraphException("Destination vertex do not exist");
	}
	

	try{
		DigraphVertex<VertexInfo, EdgeInfo>& fromVertex_list = map.at(fromVertex);

		if (std::find_if(fromVertex_list.edges.begin(), fromVertex_list.edges.end(), [toVertex](const DigraphEdge<EdgeInfo>& item){return item.toVertex == toVertex;}) != fromVertex_list.edges.end()){
			throw DigraphException("Edge already exists");
		}
		// std::cout<<fromVertex_list.edges.size()<<std::endl;
		fromVertex_list.edges.push_back(DigraphEdge<EdgeInfo>{fromVertex, toVertex, einfo});
		// std::cout<<fromVertex_list.edges.size()<<std::endl;
		edge_Count++;
		

	}
	catch(const std::out_of_range& e){
		throw DigraphException("Origin Vertex do not exist");
	}
	// std::cout<< map.at(fromVertex).edges.size()<<std::endl;
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeVertex(int vertex)
{
	int test = vertexCount();
	map.erase(vertex);
	if (vertexCount() == test){
		throw DigraphException("Specified vertex do not exist");
	}
	auto pred = [vertex](const DigraphEdge<EdgeInfo>& item){return item.toVertex == vertex;};

	for (auto i = map.begin(); i != map.end(); i++){
		auto finder = std::find_if(i->second.edges.begin(), i->second.edges.end(), pred);
		if (finder != i->second.edges.end()){
			i->second.edges.erase(finder);
		}
	}

}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeEdge(int fromVertex, int toVertex)
{

	if (not exists_in_map(toVertex) or not exists_in_map(fromVertex)){
		throw DigraphException("Destination or Origin Vertex not found");
	}
	DigraphVertex<VertexInfo, EdgeInfo>& target = map.at(fromVertex);
	auto pred = [toVertex](const DigraphEdge<EdgeInfo>& item){return item.toVertex == toVertex;};
	auto result = std::find_if(target.edges.begin(), target.edges.end(), pred);
	if (result == target.edges.end()){
		throw DigraphException("Edge not exist");
	}
	target.edges.erase(result);

}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::vertexCount() const noexcept
{
    return map.size();
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount() const noexcept
{
    return edge_Count;
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount(int vertex) const
{
    if (not exists_in_map(vertex)){
    	throw DigraphException("Vertex not found");
    }
    return map.at(vertex).edges.size();
}


template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::isStronglyConnected() const
{
    std::vector<int> allV = vertices();
    if (allV.size() < 2){
        return true;
    }
    for (int i = 0; i < allV.size(); i++){
    	if (DFT_one(allV[i]) != vertexCount()){
    		// std::cout << allV[i] << " " << DFT_one(allV[i]) << std::endl;
    		return false;
    	}
    }
    return true;
}


template <typename VertexInfo, typename EdgeInfo>
std::map<int, int> Digraph<VertexInfo, EdgeInfo>::findShortestPaths(
    int startVertex,
    std::function<double(const EdgeInfo&)> edgeWeightFunc) const
{
	std::priority_queue<std::pair<int, double>, std::vector<std::pair<int, double>>, 
	std::function<bool(std::pair<int, double>, std::pair<int, double>)>> 
	pq{[](std::pair<int, double> f, std::pair<int, double> s){return f.second > s.second;}};

	std::vector<int> V = vertices();
	std::unordered_set<int> k_value;
	for (int i = 0; i < V.size(); i++){
		k_value.insert(V[i]);
	}

	std::map<int, int> p_value;

	std::map<int, double> d_value;
	pq.push(std::pair<int, double>{startVertex, 0});
	d_value.insert(std::pair<int, double>{startVertex, 0});
	p_value.insert(std::pair<int, int>{startVertex, startVertex});
	while (pq.size() != 0){
		std::pair<int, double> target = pq.top();
		pq.pop();
		if (k_value.find(target.first) != k_value.end()){
			k_value.erase(target.first);

			std::vector<std::pair<int, int>> target_edges = edges(target.first);
			for (int i = 0; i < target_edges.size(); i++){
				double newValue = target.second + edgeWeightFunc(edgeInfo(target_edges[i].first,target_edges[i].second));
				if (d_value.count(target_edges[i].second) == 0 or d_value.at(target_edges[i].second) > newValue){
					d_value[target_edges[i].second] = newValue;
					p_value[target_edges[i].second] = target.first;
					pq.push(std::pair<int, double>{target_edges[i].second, newValue});

				}
			}

		}
	}
	for (auto i = k_value.begin(); i != k_value.end(); i++){
		p_value.insert(std::pair<int, int>{*i, *i});
	}

    return p_value;
}

template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::exists_in_map(int vertex) const{
	return map.find(vertex) != map.end();
}

// template <typename VertexInfo, typename EdgeInfo>
// void Digraph<VertexInfo, EdgeInfo>::visualize(){
// 	auto it = map.begin();
// 	while (it != map.end()){
// 		std::cout << it->second.vinfo << "(" << it->first << "): ";
// 		// std::cout<<it->second.vinfo<<it->second.edges.size()<<std::endl;
// 		auto small_it = it->second.edges.begin();
// 		while (small_it != it->second.edges.end()){
// 			std::cout << small_it->toVertex << "(" << small_it -> einfo << "); ";
// 			small_it++;
// 		}
// 		std::cout<<std::endl;
// 		it ++;
// 	}
// }

template <typename VertexInfo, typename EdgeInfo>
unsigned int Digraph<VertexInfo, EdgeInfo>::DFT_one(int Vertex) const{
	std::unordered_set<int> visited{Vertex};

	DFTr(Vertex, visited);
	// std::cout << Vertex << "; size = "<< visited.size() <<std::endl;
	return visited.size();
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::DFTr(int vertex, std::unordered_set<int>& visited_set) const{
	std::vector<std::pair<int, int>> reachable = edges(vertex);
	for (int i = 0; i < reachable.size(); i++){
		if (visited_set.find(reachable[i].second) == visited_set.end()){
			visited_set.insert(reachable[i].second);
			DFTr(reachable[i].second, visited_set);
		}
	}
}
// template <typename VertexInfo, typename EdgeInfo>
// bool Digraph<VertexInfo, EdgeInfo>::compare_less(std::pair<int, int> one, std::pair<int, int> another_one){
//     return one.second > another_one.second;
// }
#endif // DIGRAPH_HPP

