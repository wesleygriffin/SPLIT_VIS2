//	IsoSurface II
//	Hamish Carr, 2003

//	PriorityIndex.h:		class representing an indirect pointer for a Priority Queue

//	little class for working with a priority queue
	
class PriorityIndex
	{ // class PriorityIndex
	public:
	long index;													//	index to the item this entry represents
	long priority;													//	the priority of the item

	PriorityIndex(long Index, long Priority)							//	constructor
		{ // constructor
		index = Index;
		priority = Priority;
		} // constructor
	}; // class PriorityIndex

//	the comparison class necessary for the STL to work
//	uses greater than to get priority queue removing low numbers first
class PriorityCompare
	{ // class PriorityCompare
	public:
	bool operator () (PriorityIndex index1, PriorityIndex index2)			//	comparison operator
		{ // comparison operator
		return index1.priority > index2.priority;						//	pretty straightforward
		} // comparison operator
	}; // class PriorityCompare

