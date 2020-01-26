#include "gtest/gtest.h"
#include "suffix_array.h"
#include "pretty_print.h"

namespace alg {
namespace {

TEST(SuffixArray,SimpleCompress){

  std::vector<unsigned int> v = {2,3,2,4,23,13,12,5,13,13,12,12,11,5,4};
  EXPECT_EQ(alg::single_compress(v.begin(),v.end(),v.begin()),8);
  EXPECT_EQ(v,std::vector<unsigned int>({0, 1, 0, 2, 7, 6, 5, 3, 6, 6, 5, 5, 4, 3, 2}));
}

TEST(SuffixArray,RadixCompressUnsigned){
  std::vector<unsigned int> v = {2,3,2,4,23,13,12,5,13,13,12,12,11,5,4};
  EXPECT_EQ(alg::radix_compress(v.begin(),v.end(),v.begin()),8);
  EXPECT_EQ(v,std::vector<unsigned int>({0, 1, 0, 2, 7, 6, 5, 3, 6, 6, 5, 5, 4, 3, 2}));
}

TEST(SuffixArray,RadixCompressSigned){
  std::vector<int> v = {2,3,2,4,23,13,12,-5,-13,-13,12,12,11,5,4};
  std::vector<uint16_t> vc(v.size());
  EXPECT_EQ(alg::radix_compress(v.begin(),v.end(),vc.begin()),10);
  EXPECT_EQ(vc,std::vector<uint16_t >({2, 3, 2, 4, 9, 8, 7, 1, 0, 0, 7, 7, 6, 5, 4}));
}

TEST(SuffixArray,SuffixBlockCompress){
  std::vector<uint16_t> v = {2,1,3,1,3,1};
  EXPECT_EQ(alg::block_radix_compress<2>(v.begin(),v.end(),v.begin()),4);
  EXPECT_EQ(v,std::vector<uint16_t >({2,1,3,1,3,0}));
}

TEST(SuffixArray,SuffixCompressNlogN){
  std::vector<uint16_t> v = {2,1,3,1,3,1};
  alg::compress_suffix_nlogn(v.begin(),v.end(),v.begin());
  EXPECT_EQ(v,std::vector<uint16_t >({3,2,5,1,4,0}));
}

TEST(SuffixArray,SuffixCompressInsertion){
  std::vector<uint16_t> v = {2,1,3,1,3,1};
  alg::compress_suffix_insertion(v.begin(),v.end(),v.begin());
  EXPECT_EQ(v,std::vector<uint16_t >({3,2,5,1,4,0}));
}

TEST(SuffixArray,SuffixCompressInsertionSmall){
  std::vector<uint16_t> v = {1,3,4,0};
  alg::qd_suffix_insertion(v.begin(),v.size());
  EXPECT_EQ(v,std::vector<uint16_t >({3,0,1,2}));
}

TEST(SuffixArray,SuffixArrayKsBanana){
  std::vector<uint16_t> v = {2,1,3,1,3,1};
  alg::ks_suffix_array(v.begin(),v.size());
  EXPECT_EQ(v,std::vector<uint16_t >({5,3,1,0,4,2}));
}

TEST(SuffixArray,SuffixArrayKsAs){
  std::vector<uint16_t> v(10,0);
  alg::ks_suffix_array(v.begin(),v.size());
  EXPECT_EQ(v,std::vector<uint16_t >({9,8,7,6,5,4,3,2,1,0}));
}

TEST(SuffixArray,SuffixArrayKsAAAs){
  constexpr int n = 1000;
  std::vector<unsigned int> v(n,0);
  alg::ks_suffix_array(v.begin(),n);
  std::vector<unsigned int> vsa(n,0);
  std::iota(vsa.rbegin(),vsa.rend(),0);
  EXPECT_EQ(v,vsa);
}

TEST(SuffixArray,SuffixArrayBanana){
  constexpr std::string_view s = "banana";
  std::vector<unsigned int> sa(s.size());
  alg::suffix_array(s.begin(),s.end(),sa.begin());
  EXPECT_EQ(sa,std::vector<unsigned int>({5,3,1,0,4,2}));
}

TEST(SuffixArray,SuffixArrayMississippi){
  constexpr std::string_view s = "mississippi";
  std::vector<unsigned int> sa(s.size());
  alg::suffix_array(s.begin(),s.end(),sa.begin());
  EXPECT_EQ(sa,std::vector<unsigned int>({10, 7, 4, 1, 0, 9, 8, 6, 3, 5, 2}));
}

TEST(SuffixArray,SuffixArrayA){
  constexpr std::string_view s = "a";
  std::vector<unsigned int> sa(s.size());
  alg::suffix_array(s.begin(),s.end(),sa.begin());
  EXPECT_EQ(sa,std::vector<unsigned int>({0}));
}

}
}