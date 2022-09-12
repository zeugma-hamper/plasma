
/* (c)  oblong industries */

#ifndef CRAWL_ITERATOR_COEXIST
#define CRAWL_ITERATOR_COEXIST

#include "libLoam/c++/ObCrawl.h"

namespace oblong {
namespace loam {

template <typename ELEMType, typename CRAWLRetType>
class CrawlIterator
{
 OB_PRIVATE:
  ObCrawl<ELEMType> crawl;

  typedef typename ObCrawl<ELEMType>::OC_Guts OC_Guts;

 public:
  /**
   * Construct a CrawlIterator which is end().  i. e. it is equal to
   * a CrawlIterator which has been exhausted/is empty.
   */
  CrawlIterator () {}  // gets the default crawl, which is empty

  /**
   * Construct a CrawlIterator which is begin().  Give it an OC_Guts
   * which will crawl your container.  It assumes ownership of the guts.
   */
  CrawlIterator (OC_Guts *guts) : crawl (guts) {}

  // relying on the compiler for copy and move constructors and assignments

  /**
   * Extremely lame equality operator.  Returns true if both iterators
   * are empty (i. e. end) or if both iterators are non-empty (not end).
   * Returns false otherwise.  This is not what an equality operator
   * should do, but it is sufficient to implement a for loop.  And
   * since crawls don't offer comparison operators, we can't do any
   * better as long as we are built on top of a crawl.
   */
  bool operator== (const CrawlIterator<ELEMType, CRAWLRetType> &o) const
  {
    return crawl.isempty () == o.crawl.isempty ();
  }

  /**
   * Extremely lame inequality operator.
   */
  bool operator!= (const CrawlIterator<ELEMType, CRAWLRetType> &o) const
  {
    return crawl.isempty () != o.crawl.isempty ();
  }

  /**
   * Return current element.
   */
  CRAWLRetType operator* () const { return crawl.fore (); }

  /**
   * Move to next element.  Prefix version.
   */
  CrawlIterator<ELEMType, CRAWLRetType> &operator++ ()
  {
    crawl.popfore ();
    return *this;
  }

  /**
   * Move to next element.  Postfix version.  Shockingly
   * inefficient, so please use the prefix version instead.
   */
  CrawlIterator<ELEMType, CRAWLRetType> operator++ (int)
  {
    /* Making a copy of our iterator means copying the guts
     * of the crawl, which means allocating memory.  Ugh! */
    CrawlIterator<ELEMType, CRAWLRetType> ret = *this;
    crawl.popfore ();
    return ret;
  }
};
}
}  // final resting place of namespaces loam and oblong...

#endif /* CRAWL_ITERATOR_COEXIST */
