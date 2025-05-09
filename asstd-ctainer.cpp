/*   (c)2024 Vainstein K.   */
#include "common.h"

#include <stdlib.h> // for srand() and rand()
#include <limits.h> // for UINT_MAX
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>


using setU_t = std::set<unsigned>;
using usetU_t = std::unordered_set<unsigned>;
using mapFCs_t = std::map<float,const char*>;
using umapFCs_t = std::unordered_map<float,const char*>;

/* Retval of *node-handle insert* is a struct with at least the following ivars,
     {
		someAssocCtainer::iterator   position;
		bool                         inserted;
		someAssocCtainer::node_type  node;
     }
   Unlike noha itself, insert retval has no nameable type; so, must auto.
*/

void pr_map (mapFCs_t const& m, const char *tag, int callerLn) {
	printf("(Ln%d)   \e[33;3m%s\e[0m --- {\n", callerLn, tag);
	for (auto const& kv : m) {
		printf("\t%-6.2f  -->  %s\n", kv.first, kv.second);
	}
	printf("}  // %zu elems.\n", m.size());
}
#define PRmap(m,tag) pr_map((m),(tag),__LINE__)


void test__node_manip () // All the node-handle stuff is 17+. 
{	PRenteredFU;
	setU_t sa{55, 22, 99, 77};
#if VER_ge17
	setU_t::node_type nohaEmpty_dfltCstructd;
	setU_t::node_type nohaEmpty_fromLookup = sa.extract(66);
	assert(  sa.count(55));
	setU_t::node_type noha55               = sa.extract(55);
	assert(! sa.count(55));

	assert(! (bool)nohaEmpty_dfltCstructd);
	assert(! (bool)nohaEmpty_fromLookup);
	assert(  (bool)noha55);

	setU_t sb{11, 55, 88}; // Splicing a "55" node into here ought to fail.
	setU_t::iterator sb__it55 = sb.find(55); // Not ::const_iterator, because shall want to...
	assert(sb__it55 != sb.end());            // ...compare to non-const iterator later.

	auto ret00 = sb.insert(std::move(nohaEmpty_dfltCstructd)); // ins empty ==> noop.
	assert(! ret00.inserted);
	assert(ret00.position == sb.end());
	auto ret01 = sb.insert(std::move(nohaEmpty_fromLookup)); // ins empty ==> noop.
	assert(! ret01.inserted);
	assert(ret01.position == sb.end());

	assert(! sa.count(55));
	assert((bool)noha55); // Make sure it's still valid.
	auto ret02 = sb.insert(std::move(noha55)); // violate uniqueness ==> noop.
	assert(! ret02.inserted);
	assert(ret02.position == sb__it55);
	assert(! sa.count(55));

	setU_t sc{11, 33, 88}; // Splicing a "55" node into here ought to succeed.

	auto ret03 = sc.insert(std::move(noha55)); // violate uniqueness ==> noop.
	assert(! ret03.inserted);
	assert(ret03.position == sc.end());
	assert(! (bool)noha55); // Oughtn't be valid now, since we used it up.
#endif
}

void test__node_manip__map () // Maybe this will work better.
{	PRenteredFU;
	mapFCs_t ma{ {3.33F,"ee"} , {9.99F,"ww"} , {5.55F,"jj"} , {6.66F,"mm"} };
	mapFCs_t mb{ {2.22F,"bb"} , {11.1F,"yy"} , {5.55F,"kk"} , {4.44F, "hh"} };
#if VER_ge17
	mapFCs_t::node_type noha666 = ma.extract(6.66F);
	assert(  (bool)noha666);
	auto ret02 = mb.insert(std::move(noha666));
	assert(  ret02.inserted);
	assert(! (bool)noha666);

	assert(  ma.count(5.55F));
	SAYevalPRret( ma.count(5.55F)  ,"%zu");
	PRmap(ma, "ma pre-extract of 5.55");
	mapFCs_t::node_type noha555 = ma.extract(5.55F);
	PRmap(ma, "ma post-extract of 5.55");
	PRbool(noha555);
	PRbool(ma.count(5.55F));
	PRbool(ma.find(5.55F) != ma.cend());
	SAYevalPRret( ma.count(5.55F)  ,"%zu");
	assert(  (bool)noha555); // Make sure it's valid, to begin with.
	assert(! ma.count(5.55F));
	assert(ma.find(5.55F) == ma.cend());

	PRmap(mb, "mb pre-attempt to insert the extracted node");
	auto ret03 = mb.insert(std::move(noha555));
	assert(! ret03.inserted);
	assert(! (bool)noha555); // Make sure it's still valid.
	assert(! ma.count(5.55F));
	mapFCs_t::const_iterator cit = mb.find(5.55F);
	assert(cit != mb.cend());
	PRmsg("cit->second = '%s'\n", cit->second);
#endif
}

void test__node_merge ()
{	PRenteredFU;
	mapFCs_t ma{ {3.33F,"ee"} , {9.99F,"ww"} , {5.55F,"jj"} , {6.66F,"mm"} };
	mapFCs_t mb{ {2.22F,"bb"} , {11.1F,"yy"} , {5.55F,"kk"} , {4.44F, "hh"} };
	PRmap(ma, "ma, originally");
	PRmap(mb, "mb, originally");
	mb.merge(ma);
	PRmap(ma, "ma, post-merge of mb into ma");
	PRmap(mb, "mb, post-merge of mb into ma");
}


void test__bucket_manip ()
{	PRenteredFU;
	usetU_t sa(/*# of buckets*/ 4);
	PRmsg("at start, maxLF=%f currLF=%f nBkts=%zu nElems=%zu\n"
	      , sa.max_load_factor(), sa.load_factor(), sa.bucket_count(), sa.size());
	sa.max_load_factor(/*avg #elems in bucket*/ 0.75);
	sa.insert({55,69,71,72,73,22,98,76,23,56,111,0,82,81,80, 79,79 ,94,77,83,84,31,22,21,7});
	PRmsg("aft ins, maxLF=%f currLF=%f nBkts=%zu nElems=%zu\n"
	      , sa.max_load_factor(), sa.load_factor(), sa.bucket_count(), sa.size());
	sa.rehash(3);
	PRmsg("rehash(3), maxLF=%f currLF=%f nBkts=%zu nElems=%zu\n"
	      , sa.max_load_factor(), sa.load_factor(), sa.bucket_count(), sa.size());

	for (unsigned const& elem : sa) {
		usetU_t::size_type bktIdx = sa.bucket(elem);
		fprintf(stderr, "\tElem %4u  -->  bktIdx=%zu ", elem, bktIdx);
		usetU_t::size_type bktSz = sa.bucket_size(bktIdx);
		fprintf(stderr, "--> %zu elems.\n", bktSz);
	}

	usetU_t::const_local_iterator clit111 = sa.cbegin(sa.bucket(111U));
	PRmsg("Deref clit111, %u\n", *clit111);
	usetU_t::const_local_iterator clitUMAX = sa.cbegin(sa.bucket(UINT_MAX));
	PRmsg("Deref clitUMAX, %u\n", *clitUMAX);
}


void test__heterogeneousLookupAndErasure ()
{
//todo
}


int mk_pseudorandomInt ()
{
    static bool haveSeeded{false};
    if (! haveSeeded) {
        srand(42U);
        haveSeeded = true;
    }
	return rand();
}

void test__generate ()
{	PRenteredFU;
//
	std::vector<int> numbers(20); // All dflt-initialized to 0.
	auto isZero = [](int x) -> bool { return 0==x; };
	SAYevalCHKretBOOL(   std::all_of(numbers.cbegin(), numbers.cend(), isZero)   ,true);
	std::generate(numbers.begin(), numbers.end(), mk_pseudorandomInt);
	SAYevalCHKretBOOL(   std::none_of(numbers.cbegin(), numbers.cend(), isZero)   ,true);
//
	std::vector<std::string> strings{"aa", "bb", "cc", "dd", "ee"};
	assert("cc" == strings[2]);
#if VER_ge17
	decltype(strings)::const_iterator cit =
		std::generate_n(strings.begin(), 3U,
		                []()->std::string { return std::to_string(mk_pseudorandomInt()); });
	assert("dd" == *cit);
	assert("cc" != strings[2]);
#endif
	PRlit(strings[2].c_str());
}


int main()
{
	test__bucket_manip(); // Pertains only to *unordered* associative containers.

	test__node_manip(); // Pertains to all associative containers.
	test__node_manip__map();
	test__node_merge();

	test__heterogeneousLookupAndErasure();

	test__generate();
}



#if 0
TODO:

Heterogeneous lookup in unordered associative containers ????????????????

"flat" container adaptors: std::flat_map, std::flat_multimap, std::flat_set, std::flat_multiset

Associative containers heterogeneous erasure. 

#endif
