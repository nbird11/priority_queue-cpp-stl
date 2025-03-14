/***********************************************************************
 * Header:
 *    PRIORITY QUEUE
 * Summary:
 *    Our custom implementation of std::priority_queue
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *
 *    This will contain the class definition of:
 *        priority_queue          : A class that represents a Priority Queue
 * Author
 *    Nathan Bird, Brock Hoskins
 ************************************************************************/

#pragma once

#include "vector.h"  // for default underlying container
#include <stdexcept> // for out_of_range

class TestPQueue;    // forward declaration for unit test class

namespace custom
{

/*************************************************
 * P QUEUE
 * Create a priority queue.
 *************************************************/
   template<class T, class Container = custom::vector<T>, class Compare = std::less<T>>
   class priority_queue
   {
      friend class ::TestPQueue; // give the unit test class access to the privates
      template <class TT, class CContainer, class CCompare>
      friend void swap(priority_queue<TT, CContainer, CCompare>& lhs, priority_queue<TT, CContainer, CCompare>& rhs);

   public:

      //
      // construct
      //
      priority_queue(const Compare& c = Compare()) : container(Container()), compare(c)
      {}
      priority_queue(const priority_queue& rhs, const Compare& c = Compare()) : container(rhs.container), compare(c)
      {}
      priority_queue(priority_queue&& rhs, const Compare& c = Compare()) : container(std::move(rhs.container)), compare(c)
      {}
      template <class Iterator>
      priority_queue(Iterator first, Iterator last, const Compare& c = Compare()) : compare(c)
      {
         container.reserve(last - first);
         while (first != last)
         {
            push(*first);
            first++;
         }
      }
      explicit priority_queue(const Compare& c, Container&& rhs) : compare(c), container(std::move(rhs))
      {
         heapify();
      }
      explicit priority_queue(const Compare& c, Container& rhs) : compare(c), container(rhs)
      {}
      ~priority_queue()
      {
         container.clear();
      }

      //
      // Access
      //
      const T& top() const;

      //
      // Insert
      //
      void push(const T& t);
      void push(T&& t);

      //
      // Remove
      //
      void pop();

      //
      // Status
      //
      size_t size() const
      {
         return container.size();
      }
      bool empty() const
      {
         return container.empty();
      }

   private:

      void heapify();                            // convert the container in to a heap
      bool percolateDown(size_t indexHeap);      // fix heap from index down. This is a heap index!

      Container container;       // underlying container (probably a vector)
      Compare   compare;         // comparision operator

   };

   /************************************************
    * P QUEUE :: TOP
    * Get the maximum item from the heap: the top item.
    ***********************************************/
   template <class T, class Container, class Compare>
   const T& priority_queue<T, Container, Compare>::top() const
   {
      if (!empty())
         return container.front();
      throw std::out_of_range("std:out_of_range");
   }

   /**********************************************
    * P QUEUE :: POP
    * Delete the top item from the heap.
    **********************************************/
   template <class T, class Container, class Compare>
   void priority_queue<T, Container, Compare>::pop()
   {
      if (empty()) return;

      using std::swap;  // Brings std::swap into scope as a fallback,
                        // and allows ADL to find a better match
      swap(container[0], container[size() - 1]);
      container.pop_back();
      percolateDown(1 /*heap index*/);
   }

   /*****************************************
    * P QUEUE :: PUSH
    * Add a new element to the heap, reallocating as necessary
    ****************************************/
   template <class T, class Container, class Compare>
   void priority_queue<T, Container, Compare>::push(const T& t)
   {
      container.push_back(t);
      size_t index = container.size() / 2;
      while (index && percolateDown(index))
         index = index / 2;
   }
   template <class T, class Container, class Compare>
   void priority_queue<T, Container, Compare>::push(T&& t)
   {
      container.push_back(std::move(t));
      size_t index = container.size() / 2;
      while (index && percolateDown(index))
         index = index / 2;
   }

   /************************************************
    * P QUEUE :: PERCOLATE DOWN
    * The item at the passed index may be out of heap
    * order. Take care of that little detail!
    * Return TRUE if anything changed.
    ************************************************/
   template <class T, class Container, class Compare>
   bool priority_queue<T, Container, Compare>::percolateDown(size_t indexHeap)
   {
      size_t indexLeft = indexHeap * 2;
      size_t indexRight = indexLeft + 1;

      size_t indexBigger = 0;

      if (indexRight <= size() && compare(container[indexLeft - 1], container[indexRight - 1]))
         indexBigger = indexRight;
      else
         indexBigger = indexLeft;

      if (indexBigger <= size() && compare(container[indexHeap - 1], container[indexBigger - 1]))
      {
         using std::swap;  // Brings std::swap into scope as a fallback,
                           // and allows ADL to find a better match
         swap(container[indexHeap - 1], container[indexBigger - 1]);
         percolateDown(indexBigger);
         return true;
      }

      return false;
   }

   /************************************************
    * P QUEUE :: HEAPIFY
    * Turn the container into a heap.
    ************************************************/
   template <class T, class Container, class Compare>
   void priority_queue<T, Container, Compare>::heapify()
   {
      size_t num = container.size();
      for (size_t indexHeap = num / 2; indexHeap >= 1; indexHeap--)
         percolateDown(indexHeap);
   }

   /************************************************
    * SWAP
    * Swap the contents of two priority queues
    ************************************************/
   template <class T, class Container, class Compare>
   inline void swap(custom::priority_queue<T, Container, Compare>& lhs,
                    custom::priority_queue<T, Container, Compare>& rhs)
   {
      std::swap(lhs.container, rhs.container);
      std::swap(lhs.compare, rhs.compare);
   }

};