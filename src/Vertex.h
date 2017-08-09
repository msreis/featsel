#ifndef VERTEX_H_
#define VERTEX_H_

#include "global.h"
#include "Element.h"

class Vertex 
{

private:
	
	Vertex * lo;
	Vertex * hi;
	Element * var;
	bool value;
	unsigned int id;		// just an integer for identification
	unsigned int index; 	// the index of *var at the ordering plus one
	
public:

	bool mark;

	// Default Vertex constructor
	//
	Vertex ();

	// Constructor for nonterminal vertices
	//
	Vertex (Element *, unsigned int);


	// Constructor for terminal vertices
	//
	Vertex (bool, unsigned int);


	// Default destructor.
	//
	virtual ~Vertex ();

	// Returns the element of a vertex
	//
	Element * get_var ();

	// Sets the element of a vertex
	//
	void set_var (Element *);

	// Returns the child. True for high (right) and False for low (left)
	//
	Vertex * get_child (bool);


	// Sets the child. True for high (right) and False for low (left)
	//
	void set_child (Vertex *, bool);


	// Returns the value of a vertex
	//
	int get_value (); 


	// Sets the value of a vertex
	//
	void set_value (int);
	
	// Returns the id of a vertex
	//
	unsigned int get_id ();


	// Sets the id of a vertex
	//
	void set_id (unsigned int);

	// Returns the index of a vertex
	//
	unsigned int get_index ();


	// Sets the index of a vertex
	//
	void set_index (unsigned int);	


	// Says if the vertex is terminal
	//
	bool is_terminal();

};

#endif /* VERTEX_H_ */