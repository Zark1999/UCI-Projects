 // SkipListSet.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// A SkipListSet is an implementation of a Set that is a skip list, implemented
// as we discussed in lecture.  A skip list is a sequence of levels
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the keys and their
// values.  Instead, you'll need to implement your own dynamically-allocated
// nodes, with pointers connecting them.  You can, however, use other parts of
// the C++ Standard Library -- including <random>, notably.
//
// Each node should contain only two pointers: one to the node that follows it
// on the same level and another to the equivalent node on the level below it.
// Additional pointers use more memory but don't enable any techniques not
// enabled by the other two.
//
// A couple of utilities are included here: SkipListKind and SkipListKey.
// You can feel free to use these as-is and probably will not need to
// modify them, though you can make changes to them, if you'd like.

#ifndef SKIPLISTSET_HPP
#define SKIPLISTSET_HPP

#include <memory>
#include <random>
#include "Set.hpp"




// SkipListKind indicates a kind of key: a normal one, the special key
// -INF, or the special key +INF.  It's necessary for us to implement
// the notion of -INF and +INF separately, since we're building a class
// template and not all types of keys would have a reasonable notion of
// -INF and +INF.

enum class SkipListKind
{
    Normal,
    NegInf,
    PosInf
};




// A SkipListKey represents a single key in a skip list.  It is possible
// to compare these keys using < or == operators (which are overloaded here)
// and those comparisons respect the notion of whether each key is normal,
// -INF, or +INF.

template <typename ElementType>
class SkipListKey
{
public:
    SkipListKey(SkipListKind kind, const ElementType& element);

    bool operator==(const SkipListKey& other) const;
    bool operator<(const SkipListKey& other) const;
	
	ElementType value() const;

private:
    SkipListKind kind;
    ElementType element;
};


template <typename ElementType>
SkipListKey<ElementType>::SkipListKey(SkipListKind kind, const ElementType& element)
    : kind{kind}, element{element}
{
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator==(const SkipListKey& other) const
{
    return kind == other.kind
        && (kind != SkipListKind::Normal || element == other.element);
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator<(const SkipListKey& other) const
{
    switch (kind)
    {
    case SkipListKind::NegInf:
        return other.kind != SkipListKind::NegInf;

    case SkipListKind::PosInf:
        return false;

    default: // SkipListKind::Normal
        return other.kind == SkipListKind::PosInf
            || (other.kind == SkipListKind::Normal && element < other.element);
    }
}

template <typename ElementType>
ElementType SkipListKey<ElementType>::value() const
{
	return element;
}

// The SkipListLevelTester class represents the ability to decide whether
// a key placed on one level of the skip list should also occupy the next
// level.  This is the "coin flip," so to speak.  Note that this is an
// abstract base class with one implementation, RandomSkipListLevelTester,
// just below it.  RandomSkipListLevelTester is what it sounds like: It
// makes the decision at random (with a 50/50 chance of deciding whether
// a key should occupy the next level).  However, by setting things up
// this way, we have a way to control things more carefully in our
// testing (as you can, as well).
//
// DO NOT MAKE CHANGES TO THE SIGNATURES OF THE MEMBER FUNCTIONS OF
// THE "level tester" CLASSES.  You can add new member functions or even
// whole new level tester classes, but the ones declared below are part
// of how we test your implementation, so they need to maintain the
// parts of the interface that are declared already.

template <typename ElementType>
class SkipListLevelTester
{
public:
    virtual ~SkipListLevelTester() = default;

    virtual bool shouldOccupyNextLevel(const ElementType& element) = 0;
    virtual std::unique_ptr<SkipListLevelTester<ElementType>> clone() = 0;
};


template <typename ElementType>
class RandomSkipListLevelTester : public SkipListLevelTester<ElementType>
{
public:
    RandomSkipListLevelTester();
    virtual ~RandomSkipListLevelTester() = default;

    virtual bool shouldOccupyNextLevel(const ElementType& element) override;
    virtual std::unique_ptr<SkipListLevelTester<ElementType>> clone() override;

private:
    std::default_random_engine engine;
    std::bernoulli_distribution distribution;
};


template <typename ElementType>
RandomSkipListLevelTester<ElementType>::RandomSkipListLevelTester()
    : engine{std::random_device{}()}, distribution{0.5}
{
}


template <typename ElementType>
bool RandomSkipListLevelTester<ElementType>::shouldOccupyNextLevel(const ElementType& element)
{
    return distribution(engine);
}


template <typename ElementType>
std::unique_ptr<SkipListLevelTester<ElementType>> RandomSkipListLevelTester<ElementType>::clone()
{
    return std::unique_ptr<SkipListLevelTester<ElementType>>{
        new RandomSkipListLevelTester<ElementType>};
}




template <typename ElementType>
class SkipListSet : public Set<ElementType>
{
public:
    // Initializes an SkipListSet to be empty, with or without a
    // "level tester" object that will decide, whenever a "coin flip"
    // is needed, whether a key should occupy the next level above.
    SkipListSet();
    explicit SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester);

    // Cleans up the SkipListSet so that it leaks no memory.
    virtual ~SkipListSet() noexcept;

    // Initializes a new SkipListSet to be a copy of an existing one.
    SkipListSet(const SkipListSet& s);

    // Initializes a new SkipListSet whose contents are moved from an
    // expiring one.
    SkipListSet(SkipListSet&& s) noexcept;

    // Assigns an existing SkipListSet into another.
    SkipListSet& operator=(const SkipListSet& s);

    // Assigns an expiring SkipListSet into another.
    SkipListSet& operator=(SkipListSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a SkipListSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function runs in an expected time
    // of O(log n) (i.e., over the long run, we expect the average to be
    // O(log n)) with very high probability.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in an expected time of O(log n)
    // (i.e., over the long run, we expect the average to be O(log n))
    // with very high probability.
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // levelCount() returns the number of levels in the skip list.
    unsigned int levelCount() const noexcept;


    // elementsOnLevel() returns the number of elements that are stored
    // on the given level of the skip list.  Level 0 is the bottom level;
    // level 1 is the one above level 0; and so on.  If the given level
    // doesn't exist, this function returns 0.  (Note that the -INF
    // and +INF shouldn't be counted.)
    unsigned int elementsOnLevel(unsigned int level) const noexcept;


    // isElementOnLevel() returns true if the given element is on the
    // given level, false otherwise.  Level 0 is the bottom level; level 1
    // is the one above level 0; and so on.  If the given level doesn't
    // exist, this function returns false.
    bool isElementOnLevel(const ElementType& element, unsigned int level) const;


private:
	struct Node
	{
		SkipListKey<ElementType> key;
		Node* next;
		Node* below;
	};
	
    std::unique_ptr<SkipListLevelTester<ElementType>> levelTester;
	Node** levels;
	int height;
	int count;
	
	void deleteAll(Node** ls, int h);
	Node** copyAll(Node** ls, int h);
	Node* indexOnLevel(const SkipListKey<ElementType>& x, int h);
};



template <typename ElementType>
SkipListSet<ElementType>::SkipListSet()
: levelTester{std::make_unique<RandomSkipListLevelTester<ElementType>>()}
{
	ElementType e;
	try
	{
		height = 0;
		count = 0;
		Node* inf = new Node{SkipListKey<ElementType>{SkipListKind::PosInf,e},nullptr,nullptr};
		Node* nef = new Node{SkipListKey<ElementType>{SkipListKind::NegInf,e},inf,nullptr};
		levels = new Node*[1];
		levels[0] = nef;
	}
	catch (...)
	{}
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester)
    : levelTester{std::move(levelTester)}
{
	ElementType e;
	try
	{
		height = 0;
		count = 0;
		Node* inf = new Node{SkipListKey<ElementType>{SkipListKind::PosInf,e},nullptr,nullptr};
		Node* nef = new Node{SkipListKey<ElementType>{SkipListKind::NegInf,e},inf,nullptr};
		levels = new Node*[1];
		levels[0] = nef;
	}
	catch (...)
	{}
}


template <typename ElementType>
SkipListSet<ElementType>::~SkipListSet() noexcept
{
	deleteAll(levels,height);
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(const SkipListSet& s)
: levelTester{std::make_unique<RandomSkipListLevelTester<ElementType>>()}
{
	height = s.height;
	count = s.count;
	levels = copyAll(s.levels,s.height);
//	levelTester = s.levelTester->clone();
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(SkipListSet&& s) noexcept
{
	levels = s.levels;
	height = s.height;
	count = s.count;
	levelTester = std::move(s.levelTester);
	ElementType e;
	try
	{
		Node* inf = new Node{SkipListKey<ElementType>{SkipListKind::PosInf,e},nullptr,nullptr};
		Node* nef = new Node{SkipListKey<ElementType>{SkipListKind::NegInf,e},inf,nullptr};
		s.levels = new Node*[1];
		s.levels[0] = nef;
		s.height = 0;
		s.count = 0;
	}
	catch (...)
	{}
}


template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(const SkipListSet& s)
{
	if (this != &s)
	{
		Node** new_arr = copyAll(s.levels,s.height);
		deleteAll(levels,height);
		height = s.height;
		count = s.count;
		levelTester = s.levelTester -> clone();

		levels = new_arr;
	}
	
    return *this;
}


template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(SkipListSet&& s) noexcept
{
	if (this != &s)
	{
		deleteAll(levels,height);
		levels = s.levels;
		height = s.height;
		count = s.count;
		levelTester = std::move(s.levelTester);
		ElementType e;
		try
		{
			Node* inf = new Node{SkipListKey<ElementType>{SkipListKind::PosInf,e},nullptr,nullptr};
			Node* nef = new Node{SkipListKey<ElementType>{SkipListKind::NegInf,e},inf,nullptr};
			s.levels = new Node*[1];
			s.levels[0] = nef;
			s.height = 0;
			s.count = 0;
		}
		catch (...)
		{}
	}
	
    return *this;
}


template <typename ElementType>
bool SkipListSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void SkipListSet<ElementType>::add(const ElementType& element)
{
	SkipListKey<ElementType> x(SkipListKind::Normal,element);
	Node* curr = levels[0];
	Node* prev;
	while (curr->key < x)
	{
		prev = curr;
		curr = curr -> next;
	}
	
	if (curr->key == x)
	{}
	else
	{
		int l = 0;
		while (levelTester->shouldOccupyNextLevel(element))
		{l ++;}
		if(l>height)
		{
			Node** new_arr = new Node*[l+1];
			for (unsigned int i=0; i<l+1; i++)
			{
				if (i <= height)
				{
					new_arr[i] = levels[i];
				}
				else
				{
					ElementType e;
					try
					{
						Node* inf = new Node{SkipListKey<ElementType>{SkipListKind::PosInf,e},nullptr,new_arr[i-1]};
						Node* nef = new Node{SkipListKey<ElementType>{SkipListKind::NegInf,e},inf,new_arr[i-1]};
						new_arr[i] = nef;
					}
					catch (...)
					{}
				}
			}
			delete[] levels;
			levels = new_arr;
			height = l;
		}
		prev->next = new Node{x,curr,nullptr};
		prev = prev->next;
		
		for (unsigned int i=1; i<l+1; i++)
		{
			Node* curr2 = levels[i];
			Node* prev2;
			while (curr2->key < x)
			{
				prev2 = curr2;
				curr2 = curr2 -> next;
			}
			prev2->next = new Node{x,curr2,prev};
			prev = prev2->next;
		}
		count += 1;
	}
}

template <typename ElementType>
bool SkipListSet<ElementType>::contains(const ElementType& element) const
{
	SkipListKey<ElementType> x(SkipListKind::Normal,element);
	Node* curr = levels[height];
	while (curr != nullptr)
	{
		if (curr->key == x)
		{return true;}
		if (x < (curr->next)->key)
		{curr = curr->below;}
		else
		{curr = curr->next;}
	}
    return false;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::size() const noexcept
{
    return count;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::levelCount() const noexcept
{
    return height+1;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::elementsOnLevel(unsigned int level) const noexcept
{
	if (level <= height)
	{
		Node* curr = levels[level];
		int c = 0;
		while (curr != nullptr)
		{
			c ++;
			curr = curr -> next;
		}
		return c-2;
	}
	return 0;
}


template <typename ElementType>
bool SkipListSet<ElementType>::isElementOnLevel(const ElementType& element, unsigned int level) const
{
	if (level <= height)
	{
		SkipListKey<ElementType> x(SkipListKind::Normal,element);
		Node* curr = levels[level];
		while (curr != nullptr)
		{
			if (curr->key == x)
			{
				return true;
			}
			if (x < curr->key)
			{
				return false;
			}
			curr = curr -> next;
		}
	}
    return false;
}

template <typename ElementType>
void SkipListSet<ElementType>::deleteAll(Node** ls, int h)
{
	for (unsigned int i=0; i<h+1; i++)
	{
		Node* curr = ls[i];
		while(curr != nullptr)
		{
			Node* prev = curr;
			curr = curr -> next;
			delete prev;
		}
	}
	delete[] ls;
}
	
template <typename ElementType>
typename SkipListSet<ElementType>:: Node** SkipListSet<ElementType>::copyAll(Node** ls, int h)
{
	Node** new_arr = new Node*[h+1];
	for (unsigned int i=0; i<h+1; i++)
	{
		new_arr[i] = nullptr;
	}
	
	for (unsigned int i=0; i<h+1; i++)
	{
		Node* curr = ls[i];
		while(curr != nullptr)
		{
			Node* new_node = new Node{curr->key,nullptr,indexOnLevel(curr->key,i-1)};
			Node* curr2 = new_arr[i];
			if (curr2 == nullptr)
			{
				new_arr[i] = new_node;
			}
			else
			{
				while (curr2 -> next != nullptr)
				{
					curr2 = curr2->next;
				}
				curr2 -> next = new_node;
			}
			curr = curr -> next;
		}
	}
	return new_arr;
}

template <typename ElementType>
typename SkipListSet<ElementType>:: Node* SkipListSet<ElementType>::indexOnLevel(const SkipListKey<ElementType>& x, int h)
{
	if (h < 0) {return nullptr;}
	if (not isElementOnLevel(x.value(),h))
	{return nullptr;}
	else
	{
		Node* curr = levels[h];
		while (curr->key < x)
		{
			curr = curr -> next;
		}
		return curr;
	}
}
#endif // SKIPLISTSET_HPP

